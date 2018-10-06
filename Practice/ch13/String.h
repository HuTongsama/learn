#pragma once
#pragma warning(disable : 4996)
#include<memory>
#include<algorithm>

class String
{
public:
	String() :start(nullptr), first_free(nullptr), cap(nullptr) {}
	String(const char* str);
	String(const String& str);
	String &operator=(const String &rhs);
	String &operator+=(const String &rhs);
	char &operator[](size_t pos);
	~String();

	size_t size()const { return first_free - start; }
	size_t capacity()const { return cap - start; }
	bool empty()const { return start == first_free; }
	char *begin()const { return start; }
	char *end()const { return first_free; }
	void clear();
	void resize(size_t len);
	void reserve(size_t len);
	void shrink_to_fit();
	char &at(size_t pos);
	char &front();
	char &back();
	void push_back(const char ch);
	String& insert(size_t pos, size_t n, char c);
	String& erase(size_t pos = 0, size_t len =0);

private:
	void reallocate();
	std::pair<char *, char *>alloc_n_copy(char *begin,char *end);
	void free();
private:
	std::allocator<char>alloc;
	char *start;
	char *first_free;
	char *cap;
};
