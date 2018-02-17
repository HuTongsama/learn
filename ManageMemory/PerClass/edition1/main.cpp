#include"mem_pool.h"

int main()
{
	func *p1[10];
	funcb *p2[10];
	for (int i = 0; i < 10; i++)
	{
		p1[i] = new func;
		p2[i] = new funcb;
	}
	cout << sizeof(func) << '\t' << sizeof(funcb) << endl;
	for (int i = 0; i < 10; i++)
		cout << p1[i] << '\t' << p2[i] << endl;

	system("pause");
}
