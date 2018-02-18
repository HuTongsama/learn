#pragma once
#include<iostream>
namespace MyAlloc {
	class allocator
	{
	private:
		struct obj
		{
			obj* next;
		};
	public:
		void* allocate(size_t);
		void deallocate(void*, size_t);
	private:
		obj* freeStore = nullptr;
		const int CHUNK = 5;

	};
}
