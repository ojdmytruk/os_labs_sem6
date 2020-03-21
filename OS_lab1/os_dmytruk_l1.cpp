#include <iostream>
#include "BlockAllocator.h"
#include "Windows.h"
using namespace std;

BlockAllocator::BlockAllocator()
{
	used = new std::list<block*>(); //������ ����� ���'��, �� ����������������
	free = new std::list<block*>(); // ������ ������ ����� ���'��
	first_block = nullptr; //�������� �� ������ ���� 
	last_block = nullptr; //�������� �� ������� ����
}

/*
	������� ������ �� ���� ������� ����� size ����.
	���������� ����� ������� ����� �������� ������.
	���� ����� ���� �� �������� - ����������� nullptr
*/
void* BlockAllocator::mem_alloc(size_t size)
{
	size = correct_size(size);
	block *block = block_search(size);

	if (block == nullptr)
	{
		block = alloc_block(size);

		if (block == nullptr)
		{
			return nullptr;
		}
	}
	split_block(block, size); 
	return block->pointer; 
}

/*
	������� ������� ���� ���'�� addr �� ��������� �� ����� 
	mem_alloc(size), �� size - ����� ������� ����� � ������
*/
void* BlockAllocator::mem_realloc(void *addr, size_t size) 
{
	mem_free(addr); 
	return mem_alloc(size); 
}

/*
	������� ���������� ���'��.
	³�������� ���� addr ����������� � ������ ������������, �������� �� ������ ������
	�� �'�������� � ������� ������� �������, ���� ��� �.
*/
void BlockAllocator::mem_free(void *addr) 
{
	const auto pointer = reinterpret_cast<uint8_t*>(addr);
	for (auto block : *used) 
	{
		if (block->pointer != pointer)
			continue;
		used->remove(block);
		free->push_back(block);
		join_block(block);
		return;
	}
}

/*
	������� ��� ��������� �� ����� ��������� ����� ���'��
*/
void BlockAllocator::mem_dump()
{
	setlocale(LC_ALL, "Russian");
	auto curr_block = first_block;
	while (true) {

		cout << "���i���� ���'��i: " << curr_block->size << "\n";
		cout << "���� �����: ";
		if (contains_block(free, curr_block))
			cout << "�i�����" << endl;
		else if (contains_block(used, curr_block))
			cout << "���������������" << endl;
				
		if (curr_block->next == nullptr)
			break;

		curr_block = curr_block->next;
	}
}

/*
	������� ��� ����������� ������ �� ��� 4 �����
*/
size_t BlockAllocator::correct_size(size_t size)
{
	return (size + sizeof(int) - 1) & ~(sizeof(int) - 1);
}

/*
	������� ��� ������ ������� ������� ����� ���'��, �� �������� �� �������
*/
BlockAllocator::block* BlockAllocator::block_search(size_t size) {
	for (block *block : *free) 
	{
		if (block->size < size)
			continue;
		return block;
	}

	return nullptr; 
}

/*
	������� ��� �������� ���'�� ���������� ������� �� ����, 
	��������� ����� �� ������� �� �� ������ ������
*/
BlockAllocator::block* BlockAllocator::alloc_block(size_t size)
{
	const auto mem = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size); 

	if (mem == nullptr)
	{
		return nullptr;
	}

	const auto block_ = new block(); 
	block_->next = nullptr;
	block_->prev = nullptr;
	block_->pointer = static_cast<uint8_t*>(mem);
	block_->size = size;

	if ((free->size() == 0) && (used->size() == 0)) first_block = block_;
	free->push_back(block_); 

	if (last_block != nullptr)
	{
		last_block->next = block_;
		block_->prev = last_block;
	}

	last_block = block_;

	return block_;
}

/*
	����������� ������ �� �������� ���� ����� �� �������, ��������� ����� ����
*/
void BlockAllocator::split_block(block *block_, const size_t size) 
{
	free->remove(block_); 
	used->push_back(block_); 

	if (block_->size == size) return; 

	const auto size_diff = block_->size - size; 

	const auto splitted_block = new block(); 
	splitted_block->pointer = block_->pointer + size;
	splitted_block->size = size_diff;
	splitted_block->prev = block_;
	splitted_block->next = block_->next;

	block_->next = splitted_block; 
	block_->size = size;
	free->push_back(splitted_block); 
}

/*
	�'������ ���� � ���������� ������. � ���������, ���� ������ ���������,
	��� ���������, ���� ������ ���������.
*/
void BlockAllocator::join_block(block *block) 
{
	if (block->prev != nullptr && (block->prev + block->prev->size == block) && contains_block(free, block->prev))
	{
		block = join_blocks(block->prev, block);
	}
	
	if (block->next != nullptr && (block + block->size == block->next) && contains_block(free, block->next))
	{
		block = join_blocks(block, block->next);
	}
}

/*
	������� ��� �'������� ���� ����� ���'��
*/
BlockAllocator::block* BlockAllocator::join_blocks(block* left, block* right) 
{
	left->size += right->size;
	left->next = right->next;
	return left;
}

/*
	�������� ������� ��������� ����� ���'�� � ������ �����
*/
bool BlockAllocator::contains_block(const std::list<block*>* list, block* block) 
{
	if (block == nullptr)
		return false;

	for (auto list_block : *list)
	{
		if (list_block->pointer == block->pointer)
			return true;
	}
	return false;
}

int main() {
	auto allocator = new BlockAllocator();
	setlocale(LC_ALL, "Russian");
	cout << "������� ������ mem_alloc (���i����� ���'��i)" << endl;
	auto memory = allocator->mem_alloc(sizeof(long));
	allocator->mem_dump();
	cout << "������� ������ mem_realloc (�������������� �� 7 ����)" << endl;
	auto memory_reall = allocator->mem_realloc(memory, 7);
	allocator->mem_dump();
	cout << "������� ������ mem_free" << endl;
	allocator->mem_free(memory);
	allocator->mem_dump();

	system("pause");
}