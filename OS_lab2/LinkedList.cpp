#define _CRT_SECURE_NO_WARNINGS
#include "LinkedList.h"

#include <iostream>
#include "stdlib.h"

LinkedList::LinkedList() {
	_init(DEFAULT_PAGE_SIZE);
}

LinkedList::LinkedList(int blockSizeKb) {
	_init(blockSizeKb);
}

void LinkedList::_init(int blockSizeKb) {
	_pageSizeKb = blockSizeKb;
	_head = NULL;
	_tail = NULL;
	_length = 0;
}

bool LinkedList::isEmpty() {
	return _head == NULL && _tail == NULL;
}

int LinkedList::getPageSizeKb() {
	return _pageSizeKb;
}

int LinkedList::getLength() {
	return _length;
}

void LinkedList::print() {
	list_block* iterator = new list_block;
	iterator = _head;

	while (iterator != NULL) {
		std::cout << iterator->address << " ";
		iterator = iterator->next;
	}
	std::cout << std::endl;
}

list_block* LinkedList::getPageAt(unsigned int position) {
	list_block* current = new list_block;
	current = _head;
	if (position > 0) {
		for (int i = 0; i < position; i++) {
			current = current->next;
		}
	}
	return current;
}



void LinkedList::addPageStart(void* address) {
	list_block* newBlock = new list_block;
	newBlock->address = address;
	newBlock->next = NULL;
	if (_length == 0) {
		_head = newBlock;
		_tail = newBlock;
	}
	else {
		newBlock->next = _head;
		_head = newBlock;
	}
	_length++;
}

void LinkedList::addPageEnd(void* address) {
	list_block* newBlock = new list_block;
	newBlock->address = address;
	newBlock->next = NULL;
	if (_length == 0) {
		_head = newBlock;
		_tail = newBlock;
	}
	else {
		list_block* secondLastBlock = getPageAt((_length == 1) ? 0 : _length - 2);
		secondLastBlock->next = newBlock;
		_tail = newBlock;
	}
	_length++;
}

void LinkedList::addPageAt(unsigned int position, void* address) {
	if (_length == 0 || position == 0) {
		addPageStart(address);
	}
	else {
		list_block* newBlock = new list_block;
		newBlock->address = address;
		list_block* beforeBlock = new list_block;
		beforeBlock = getPageAt(position - 1);
		newBlock->next = beforeBlock->next;
		beforeBlock->next = newBlock;
		_length++;
	}
}

void LinkedList::removePageStart() {
	removePageAt(0);
}

void LinkedList::removePageEnd() {
	removePageAt(_length - 1);
}

void LinkedList::removePageAt(unsigned int position) {
	if (_length != 0 && position < _length) {
		list_block* removeBlock = new list_block;
		removeBlock = getPageAt(position);
		if (_length == 1) {
			_head = NULL;
			_tail = NULL;
		}
		else {
			if (position == 0) {
				_head = removeBlock->next;
			}
			else {
				list_block* blockBefore = new list_block;
				blockBefore = getPageAt(position - 1);
				blockBefore->next = removeBlock->next;
				if (position == _length - 1) {
					_tail = blockBefore;
				}
			}
		}
		delete removeBlock;
		_length--;
	}
}