#pragma once
#pragma warning(disable : 4996)
#include<string>
#include<memory>
#include<initializer_list>
#include<algorithm>
class StrVec
{
public:
	StrVec() :elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec &str);
	StrVec(std::initializer_list<std::string>);
	StrVec &operator=(const StrVec &rhs);
	~StrVec();
	void push_back(const std::string str);
	size_t size()const { return first_free - elements; }
	size_t capacity()const { return cap - elements; }
	std::string *begin()const { return elements; }
	std::string *end()const { return first_free; }
	void reserve(size_t n);
	void resize(size_t n);

private:
	std::allocator<std::string>alloc;
	void chk_n_alloc()	{if (size() == capacity())reallocate();}
	std::pair<std::string*, std::string*>alloc_n_copy
	(const std::string  *begin, const std::string *end);
	void free();
	void reallocate();
	std::string *elements;
	std::string *first_free;
	std::string *cap;
};
