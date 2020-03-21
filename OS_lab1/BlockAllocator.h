#pragma once

#include <iostream>
#include <list>

class BlockAllocator
{
	struct block
	{
		uint8_t *pointer; //Поккачик на область пам'яті
		size_t size; //Розмір блоку пам'яті
		block *next; //Покажчик на наступний елемент
		block *prev; //Покажчик на попередній елемент
	};

public:
	explicit BlockAllocator();
	void* mem_alloc(size_t size);
	void* mem_realloc(void* addr, size_t size);
	void mem_free(void* addr);
	void mem_dump();

private:
	std::list<block*> *free;
	std::list<block*> *used;
	block* first_block;
	block* last_block;

	size_t correct_size(size_t size);
	block* block_search(size_t size);
	block* alloc_block(size_t size);
	void split_block(block* block, size_t size);
	void join_block(block* block);
	block* join_blocks(block* left, block* right);
	bool contains_block(const std::list<block*>* list, block* block);
};
