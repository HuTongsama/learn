#include<iostream>
#include<string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main()
{
	string str, lstr;
	cout << "Enter some words without whitespace characters:" << endl;
	while (cin >> str)
		lstr = lstr + str + ' ';
	cout << "The large string is\n" << lstr << endl;
	cout << "The result of separating is:" << endl;	
	int i = 0, len = lstr.size();
	while (i < len)
	{
		string tmp;
		while (i < len&&lstr[i] == ' ')i++;
		while (i < len)
		{
			if (lstr[i] == ' ')break;
			tmp += lstr[i];
			i++;
		}
		cout << tmp << endl;
	}

	system("pause");
	return 0;
}
