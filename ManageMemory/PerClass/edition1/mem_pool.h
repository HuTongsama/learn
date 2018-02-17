#pragma once
#include<iostream>
class func
{
private:
	func* next;
	static func* freestore;
	static const int chunk;
private:
	int i;

public:
	func(int x = 0) :i(x) {}
	void* operator new(size_t);
	void operator delete(void*, size_t);

};

func* func::freestore = nullptr;
const int func::chunk = 20;

void* func::operator new(size_t size)
{
	func* p;
	if (freestore == nullptr)
	{
		size_t __chunk = chunk*size;
		freestore = p =
			reinterpret_cast<func*>(new char[__chunk]);
		for (; p != &freestore[chunk - 1]; ++p)
			p->next = (p + 1);
		p->next = nullptr;
	}
	p = freestore;
	freestore = freestore->next;
	return p;
}

void func::operator delete(void* p, size_t size)
{
	(static_cast<func*>(p))->next = freestore;
	freestore = static_cast<func*>(p);
}

class funcb 
{
private:
	int i;
public:
	funcb(int x = 0) :i(x){}
};
