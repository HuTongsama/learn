#include "String.h"
String::String(const char *str):
	start(nullptr),first_free(nullptr),cap(nullptr)
{
	for (size_t i = 0; str[i] != '\0'; ++i)
		push_back(str[i]);
}
String::String(const String &str)
{
	auto data = alloc_n_copy(str.begin(), str.end());
	start = data.first;
	first_free = data.second;
	cap = first_free;
}

String & String::operator=(const String & rhs)
{
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	start = data.first;
	first_free = data.second;
	cap = first_free;
	return *this;
}

String & String::operator+=(const String & rhs)
{
	auto newLen = size() + rhs.size();
	while (newLen > capacity())
		reallocate();
	first_free = std::uninitialized_copy(rhs.begin(), rhs.end(), first_free);
	return *this;
}

char & String::operator[](size_t pos)
{
	return *(begin() + pos);
}

String::~String()
{
	free();
}

void String::clear()
{
	while (first_free != start)
		alloc.destroy(--first_free);
}

void String::resize(size_t len)
{
	while (len > capacity())
		reallocate();
	if (len > size())
		while (size() != len)
			alloc.construct(first_free++, '\0');
	else
		while (size() != len)
			alloc.destroy(--first_free);
}

void String::reserve(size_t len)
{
	if (len > capacity())
	{
		auto data = alloc.allocate(len);
		auto dest = data;
		auto elem = start;
		for (size_t i = 0; i != size(); ++i)
			alloc.construct(dest++, std::move(*elem++));
		free();
		start = data;
		first_free = dest;
		cap = start + len;
	}
}

void String::shrink_to_fit()
{
	auto len = capacity() - size();
	if (len > 0)
	{
		auto data = alloc.allocate(size());
		auto dest = data;
		auto elem = start;
		for (size_t i = 0; i!=size(); ++i)
			alloc.construct(dest++, std::move(*elem++));
		free();
		start = data;
		first_free = dest;
		cap = first_free;
	}
	
}

char & String::at(size_t pos)
{
	return *(begin() + pos);
}

char & String::front()
{
	return *begin();
}

char & String::back()
{
	return *(begin() + size());
}

void String::push_back(const char ch)
{
	if (size() == capacity())reallocate();
	alloc.construct(first_free++, ch);
}

String & String::insert(size_t pos, size_t n, char c)
{
	while (size() + n > capacity())reallocate();
	auto oldfree = first_free;
	first_free = first_free + n;
	auto iter = first_free - 1;
	while (oldfree != begin() + pos)
	{
		auto elem = *(--oldfree);
		alloc.construct(iter--, elem);
	}
	while (oldfree != iter + 1)
		alloc.construct(oldfree++, c);
	
	return *this;
}

String & String::erase(size_t pos, size_t len)
{
	auto iter = begin() + pos;
	auto oldfree = first_free;
	first_free = first_free - len;
	while (iter != begin() + pos + len)
		alloc.destroy(iter++);
	while (iter != oldfree)
	{
		alloc.construct(iter - len, std::move(*iter));
		iter++;
	}
	while (oldfree != first_free)
		alloc.destroy(--oldfree);
	return *this;
}


void String::reallocate()
{
	auto newCapacity = size() ? 2 * size() : 1;
	auto data = alloc.allocate(newCapacity);
	auto dest = data;
	auto elem = start;
	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();
	start = data;
	first_free = dest;
	cap = start + newCapacity;
}

std::pair<char*, char*> String::alloc_n_copy(char * begin, char * end)
{
	auto data = alloc.allocate(end-begin);

	return { data,std::uninitialized_copy(begin,end,data) };
}

void String::free()
{
	if (start)
	{
		std::for_each(start, first_free, [this](char &ch) {alloc.destroy(&ch); });
		alloc.deallocate(start, cap - start);
	}
}
