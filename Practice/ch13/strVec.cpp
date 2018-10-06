#include "StrVec.h"

StrVec::StrVec(const StrVec & str)
{
	auto newdata = alloc_n_copy(str.begin(), str.end());
	elements = newdata.first;
	first_free = newdata.second;
	cap = first_free;
}

StrVec::StrVec(std::initializer_list<std::string>list)
{
	for (auto ele : list)
		push_back(ele);
}

StrVec & StrVec::operator=(const StrVec & rhs)
{
	auto data = alloc_n_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = data.second;
	cap = first_free;
	return *this;
}

StrVec::~StrVec()
{
	free();
}

void StrVec::push_back(const std::string str)
{
	chk_n_alloc();
	alloc.construct(first_free++, str);
}

void StrVec::reserve(size_t n)
{
	if (n > capacity())
	{
		auto newdata = alloc.allocate(n);
		auto dest = newdata;
		auto elem = elements;

		for (size_t i = 0; i != size(); ++i)
			alloc.construct(dest++, std::move(*elem++));
		free();
		elements = newdata;
		first_free = dest;
		cap = elements + n;
	}
	
}

void StrVec::resize(size_t n)
{
	if (n > capacity())
		reserve(n);
	if (n > size())
		for (size_t i = size(); i != n; ++i)
			alloc.construct(first_free++, "");
	else
		for (size_t i = size(); i != n; --i)
			alloc.destroy(--first_free);
}

std::pair<std::string*, std::string*> StrVec::alloc_n_copy(const std::string * begin, const std::string * end)
{
	auto data = alloc.allocate(end - begin);
	
	return{ data,uninitialized_copy(begin,end,data) };
}

void StrVec::free()
{
	if (elements)
	{
		for_each(elements, first_free, [this](std::string &rhs) {alloc.destroy(&rhs); });
		alloc.deallocate(elements, cap - elements);	
	}
}

void StrVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto newdata = alloc.allocate(newcapacity);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
}
