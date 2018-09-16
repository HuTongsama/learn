#pragma once
#include<iostream>
#include<ctime>
#include<vector>
#include<string>

using std::vector;
using std::string;
template<typename T>
using func = void(*)(vector<T> &vec);
class SortTest 
{
public:
	static SortTest* GetInstance();
	template<typename T>
	static bool accuracyTest(const vector<T> &vec);
	template<typename T>
	static void performanceTest(vector<T> &vec, string sortName, func<T> sortFunc);
private:
	SortTest() {};
	SortTest(const SortTest&) = delete;
	SortTest & operator=(const SortTest&) = delete;

	static SortTest *m_test;
};

SortTest *SortTest::m_test = new SortTest();

SortTest * SortTest::GetInstance()
{
	return m_test;
}

template<typename T>
inline bool SortTest::accuracyTest(const std::vector<T>& vec)
{
	for (unsigned int i = 0; i < vec.size() - 1; ++i)
	{
		if (vec[i + 1] < vec[i])
			return false;
	}
	return true;
}

template<typename T>
inline void SortTest::performanceTest(vector<T>& vec, string sortName, func<T> sortFunc)
{
	clock_t start, end;
	double totalTime = 0.0;
	start = clock();
	sortFunc(vec);
	end = clock();
	totalTime = double(end - start) / CLOCKS_PER_SEC;
	if (accuracyTest(vec))
		std::cout << sortName << " sorted correctly and use " << totalTime << " seconds." << std::endl;
	else
		std::cout << sortName << " has wrong result and use " << totalTime << " seconds." << std::endl;
}
