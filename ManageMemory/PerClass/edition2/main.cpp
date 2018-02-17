#include"mem_pool.h"

int main()
{
	cout << sizeof(func) << endl;
    const int N = 10;
	func* p[N];

	for (int i = 0; i < N; i++)
		p[i] = new func;
	for (int i = 0; i < N; i++)
		cout << p[i] << endl;
	for (int i = 0; i < N; i++)
		delete p[i];

	system("pause");
}
