#include"myQuery.h"

int main()
{

	ifstream file("english.txt");
	QueryResult qRes;
	qRes.runQueries(file);

	system("pause");
	return 0;
}
