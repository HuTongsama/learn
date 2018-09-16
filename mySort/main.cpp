#include"SortTest.h"
#include"mySort.h"

int main()
{
	vector<int>test;
	srand(time(0));
	size_t size = 10000;
	for (size_t i = 0; i < size; i++)
		test.push_back(rand());
	vector<int>test2 = test;
	//SortTest::GetInstance()->performanceTest(test, "bubbleSort", bubbleSort<int>);
	//SortTest::GetInstance()->performanceTest(test, "insertSort", insertSort<int>);
	SortTest::GetInstance()->performanceTest(test2, "shellSort", shellSort<int>);
	SortTest::GetInstance()->performanceTest(test, "quickSort", quickSort<int>);
	system("pause");
	return 0;
}
