#include "LinkedList.h"
#include "Windows.h"

#ifndef PageAllocator_h
#define PageAllocator_h

#define MEMORY_DEFAULT_SIZE_KB 1024
#define MIN_SIZE_KB 32

struct page {
	char* startAddress;
	unsigned int sizeKb;
};

class PageAllocator {
private:
	unsigned long _size;
	char* _memory;
	LinkedList* _pages;

	void _init(int size);
	int _getListsSize();
	int _getListNo(int size);
	void _sortList(int list);
	int* _findBuddies(int list);
	void _merge(int list);
	void _mergeAll();

public:
	PageAllocator();
	PageAllocator(int size);
	unsigned long getSize();
	unsigned long getSizeKb();
	void mem_dump();
	char* getMemoryPointer();
	page* mem_alloc(int size);
	void* mem_realloc(page* addr, int size);
	void mem_free(page*);
};

#endif