#include"test.h"
using namespace std;

int main()
{
	Foo* p1[100];
	cout << "sizeof(Foo) = " << sizeof(Foo) << endl;
	for (int i = 0; i < 23; ++i)
	{
		p1[i] = new Foo(i);
		cout << p1[i] << ' ' << p1[i]->L << endl;
	}
	for (int i = 0; i < 23; ++i)
		delete p1[i];

	Goo* p2[100];
	cout << "sizeof(Goo) = " << sizeof(Goo) << endl;
	for (int i = 0; i < 17; ++i)
	{
		p2[i] = new Goo(complex<double>(i, i));
		cout << p2[i] << ' ' << p2[i]->c << endl;
	}
	for (int i = 0; i < 17; ++i)
		delete p2[i];
	system("pause");
}
