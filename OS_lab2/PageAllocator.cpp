#define _CRT_SECURE_NO_WARNINGS
#include "PageAllocator.h"
#include <iostream>
#include <math.h>
#include "stdlib.h"
#include "Windows.h"

PageAllocator::PageAllocator() {
	_init(MEMORY_DEFAULT_SIZE_KB);
}
PageAllocator::PageAllocator(int sizeKb) {
	_init(sizeKb);
}

void PageAllocator::_init(int size) {
	_size = size * 1000 * sizeof(char);
	_memory = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
	if (_memory == NULL) {
		std::cerr << "ERROR: неможливо виділити пам'ять\n";
		exit(EXIT_SUCCESS);
	}

	_pages = new LinkedList[_getListsSize()];
	for (int i = 0; i < _getListsSize(); i++) {
		_pages[i] = LinkedList(pow(2, i + log2(MIN_SIZE_KB)));
	}
	_pages[_getListNo(getSizeKb())].addPageEnd(_memory);
}

int PageAllocator::_getListsSize() {
	return _getListNo(getSizeKb()) + 1;
}

int PageAllocator::_getListNo(int size) {
	return ceil(log2(size) - log2(MIN_SIZE_KB));
}

unsigned long PageAllocator::getSize() {
	return _size;
}

unsigned long PageAllocator::getSizeKb() {
	return _size / 1000;
}

void PageAllocator::_sortList(int listNo) {
	LinkedList unsorted = _pages[listNo];
	LinkedList sorted = LinkedList(unsorted.getPageSizeKb());
	sorted.addPageStart(unsorted.getPageAt(0)->address);
	for (int i = 1; i < unsorted.getLength(); i++) {
		void* insertAddress = unsorted.getPageAt(i)->address;
		for (int j = 0; j < sorted.getLength(); j++) {
			if (insertAddress <= (sorted.getPageAt(j)->address)) {
				sorted.addPageStart(insertAddress);
				break;
			}
			else if (j == sorted.getLength() - 1 && insertAddress > (sorted.getPageAt(j)->address)) {
				sorted.addPageEnd(insertAddress);
				break;
			}
		}
	}

	_pages[listNo] = sorted;
}

int* PageAllocator::_findBuddies(int listNo) {
	LinkedList list = _pages[listNo];
	if (list.getLength() > 1) {
		int* buddies = (int*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(int)*2);
		int blockSize = list.getPageSizeKb() * 1000;
		for (int i = 0; i <= list.getLength() - 2; i++) {
			if (list.getPageAt(i)->address == (char*)list.getPageAt(i + 1)->address - blockSize) {
				buddies[0] = i;
				buddies[1] = i + 1;
				return buddies;
			}
		}
	}
	return NULL;
}

void PageAllocator::_merge(int listNo) {
	int* buddies = _findBuddies(listNo);
	while (buddies != NULL) {
		void* startAddress = _pages[listNo].getPageAt(buddies[0])->address;
		_pages[listNo].removePageAt(buddies[1]);
		_pages[listNo].removePageAt(buddies[0]);
		_pages[listNo + 1].addPageStart(startAddress);
		_sortList(listNo + 1);

		buddies = _findBuddies(listNo);
	}
}

void PageAllocator::_mergeAll() {
	for (int i = 0; i < _getListsSize(); i++) {
		_merge(i);
	}
}

void PageAllocator::mem_dump() {
	for (int i = 0; i < _getListsSize(); i++) {
		std::cout << _pages[i].getPageSizeKb()*1000 << ": ";
		_pages[i].print();
	}
}

char* PageAllocator::getMemoryPointer() {
	return _memory;
}

page* PageAllocator::mem_alloc(int size) {
	int listNo = _getListNo(size);
	bool found = false;
	list_block* allocateBlock = new list_block;
	while (!found) {
		if (_pages[listNo].getLength() > 0) {
			allocateBlock = _pages[listNo].getPageAt(0);
			_pages[listNo].removePageAt(0);
			found = true;
		}
		else if (listNo < _getListsSize()) {
			listNo++;
			if (_pages[listNo].getLength() > 0) {
				list_block* removeBlock = new list_block;
				removeBlock = _pages[listNo].getPageAt(0);
				_pages[listNo - 1].addPageStart(((char*)removeBlock->address + (_pages[listNo - 1].getPageSizeKb() * 1000)));
				_pages[listNo - 1].addPageStart(removeBlock->address);
				_pages[listNo].removePageAt(0);
				listNo = _getListNo(size);
			}
		}
		else {
			break;
		}
	}
	if (found) {
		page* foundBlock = new page;
		foundBlock->startAddress = (char*)allocateBlock->address;
		foundBlock->sizeKb = _pages[listNo].getPageSizeKb();
		return foundBlock;
	}
	else {
		return NULL;
	}
}

void PageAllocator::mem_free(page* freeBlock) {
	int listNo = _getListNo(freeBlock->sizeKb);
	_pages[listNo].addPageStart(freeBlock->startAddress);
	_sortList(listNo);
	_mergeAll();
}

void* PageAllocator::mem_realloc(page*addr, int size) {
	mem_free(addr);
	return mem_alloc(size);
}