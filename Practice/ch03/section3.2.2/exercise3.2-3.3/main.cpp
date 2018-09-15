#include<iostream>
#include<string>
/*string input operator discards whitespace character,
  it then reads characters until the next whitespace character;
  getline function reads one line until '\n'*/
int main()
{
	std::string str;
	while (std::cin >> str)
	{
		std::cout << str << std::endl;
	}
	system("pause");
	return 0;
}
