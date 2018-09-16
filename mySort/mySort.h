#pragma once

#include<vector>

using std::vector;
template<typename T>
static void swapValue(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template<typename T>
static void bubbleSort(vector<T> &vec)
{
	auto size = vec.size();
	for (unsigned int i = 0; i < size; ++i)
		for (unsigned int j = 1; j < size - i; ++j)
		{
			if (vec[j] < vec[j - 1])
				swapValue(vec[j], vec[j - 1]);
		}
}

template<typename T>
static void insertSort(vector<T> &vec)
{
	auto size = vec.size();
	for (unsigned int i = 0; i < size; ++i)
	{
		T tmp = vec[i];
		for (unsigned int j = i ; j > 0; --j)
		{
			if (vec[j - 1] > tmp)
				vec[j] = vec[j - 1];
			else
			{
				vec[j] = tmp;
				break;
			}
		}
	}
}

template<typename T>
static void shellSort(vector<T> &vec)
{
	auto size = vec.size();
	auto offset = size / 3;
	while (offset > 0)
	{
		for (unsigned int i = offset; i < size; i += offset)
		{
			T tmp = vec[i];
			for (unsigned int j = i; j > 0; j -= offset)
			{
				if (vec[j-offset] > tmp)
					vec[j] = vec[j-offset];
				else
				{
					vec[j] = tmp;
					break;
				}
			}
		}
		offset /= 2;
	}
}

template<typename T>
static void quickSort(vector<T> &vec)
{
	size_t left = 0, right = vec.size() - 1;
	__quickSort(vec, left, right);
}
template<typename T>
static void __quickSort(vector<T> &vec, size_t left, size_t right)
{
	int i = left, j = right;
	
	if (i >= j)
		return;
	T data = vec[left];
	for (i = i + 1; i <= j; ++i)
	{
		if (vec[i] > data)
		{
			swapValue(vec[i], vec[j]);
			--j;
			--i;
		}
		else
		{
			vec[i - 1] = vec[i];
		}
	}
	vec[j] = data;
	__quickSort(vec, left, j - 1);
	__quickSort(vec, j + 1, right);
}
