#ifndef LinkedList_h
#define LinkedList_h

#define DEFAULT_PAGE_SIZE 32

struct list_block {
	void* address;
	list_block* next;
};


class LinkedList {

private:


	int _pageSizeKb;
	unsigned int _length;

	void _init(int blockSizeKb);

public:

	list_block* _head;
	list_block* _tail;
	list_block* getPageAt(unsigned int position);
	LinkedList();
	LinkedList(int blockSize);
	bool isEmpty();
	int getPageSizeKb();
	int getLength();
	void print();
	void addPageStart(void* address);
	void addPageEnd(void* address);
	void addPageAt(unsigned int position, void* address);
	void removePageStart();
	void removePageEnd();
	void removePageAt(unsigned int position);
};

#endif
