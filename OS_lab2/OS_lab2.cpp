#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "PageAllocator.h"
using namespace std;

int main()
{
	PageAllocator pa = PageAllocator(2048);
	setlocale(LC_ALL, "Russian");
	cout << "Розмiр пам'ятi: " << pa.getSizeKb() << " kB" << endl;
	cout << "Мiнiмальний розмiр блоку: " << MIN_SIZE_KB << " kB" << endl;
	cout << "Стан пам'яті: " << endl;
	pa.mem_dump();
	cout << endl;
	cout << "Запит на 40 КБ для блоку 1 (демонстрацiя mem_alloc): "  << endl;
	page* block1 = pa.mem_alloc(40);
	cout << "Адреса початку блоку 1: " << (void*)block1->startAddress << endl;
	pa.mem_dump();
	cout << endl;
	cout << "Запит на 60 КБ для блоку 2 (демонстрацiя mem_alloc): " << endl;
	page* block2 = pa.mem_alloc(60);
	cout << "Адреса початку блоку 2: " << (void*)block1->startAddress << endl;
	pa.mem_dump();
	cout << endl;
	cout << "Перевизначення блоку 1 до 70 КБ (демонстрацiя mem_realloc): " << endl;
	pa.mem_realloc(block1, 60);
	pa.mem_dump();
	cout << endl;
	cout << "Звiльнення блоку 1 (демонстрацiя mem_free): " << block1->sizeKb << "KB" << endl;
	pa.mem_free(block1);
	pa.mem_dump();
	cout << endl;
	return 0;
}