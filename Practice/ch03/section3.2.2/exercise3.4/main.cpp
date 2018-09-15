#include<iostream>
#include<string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main()
{
	string str1, str2;
	cout << "Enter str1:" << endl;
	std::getline(cin, str1);
	cout << "Enter str2:" << endl;
	std::getline(cin, str2);

	if (str1 == str2)
		cout << str1 << " is equal to " << str2 << endl;
	else if (str1 > str2)
		cout << str1 << " is larger than " << str2 << endl;
	else
		cout << str2 << " is larger than " << str1 << endl;;

	if (str1.size() == str2.size())
		cout << str1 << " and " << str2 << " have the same length" << endl;
	else if (str1.size() > str2.size())
		cout << str1 << " is longer" << endl;
	else
		cout << str2 << " is longer" << endl;
		
	
	system("pause");
	return 0;
}
