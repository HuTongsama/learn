#pragma once
#include<string>
#include<complex>
#include"mem_pool.h"
class Foo
{
public:
	long L;
	std::string str;
	static MyAlloc::allocator myAlloc;
public:
	Foo() {}
	Foo(long l) :L(l) {}
	static void* operator new(size_t size)
	{
		return myAlloc.allocate(size);
	}
	static void operator delete(void* p, size_t size)
	{
		return myAlloc.deallocate(p, size);
	}
};
MyAlloc::allocator Foo::myAlloc;

class Goo
{
public:
	std::complex<double>c;
	std::string std;
	static MyAlloc::allocator myAlloc;
public:
	Goo() {}
	Goo(const std::complex<double>& x) :c(x) {}
	void* operator new(size_t size)
	{
		return myAlloc.allocate(size);
	}

	void operator delete(void* p, size_t size)
	{
		return myAlloc.deallocate(p, size);
	}
};
MyAlloc::allocator Goo::myAlloc;
