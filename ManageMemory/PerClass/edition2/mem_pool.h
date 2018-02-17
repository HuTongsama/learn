#pragma once
#include<iostream>
class func
{
private:
	struct Info
	{
		unsigned long m;
		char c;
	};

	union{
		Info inf;
		func* next;
	};
public:
	static void* operator new(size_t size);
	static void operator delete(void* p, size_t size);
private:
	static const int BLOCK_SIZE;
	static func* headOfFreeList;

};

const int func::BLOCK_SIZE = 512;
func* func::headOfFreeList = nullptr;

void* func::operator new(size_t size)
{
	if (size != sizeof(func))
		return ::operator new(size);

	func* p = headOfFreeList;

	if (!headOfFreeList)
	{
		func* newBlock = static_cast<func*>
			(::operator new(BLOCK_SIZE * sizeof(func)));

		for (int i = 0; i < BLOCK_SIZE - 1; i++)
			newBlock[i].next = &newBlock[i + 1];
		newBlock[BLOCK_SIZE - 1].next = nullptr;
		p = newBlock;
		headOfFreeList = p->next;
	}
	else
	{
		headOfFreeList = p->next;
	}
	return p;
}

void func::operator delete(void* p, size_t size)
{
	if (p == nullptr)
		return;
	if (size != sizeof(func))
	{
		::operator delete(p);
		return;
	}

	static_cast<func*>(p)->next = headOfFreeList;
	headOfFreeList = static_cast<func*>(p);
}
