#include"Sales_item.h"
int main()
{
	Sales_item itemAll;
	std::cout << "Enter the information:\n";
	if (std::cin >> itemAll)
	{
		Sales_item item;
		while (std::cin >> item)
		{
			if (item.isbn() == itemAll.isbn())
			{
				itemAll = itemAll + item;
			}
			else
			{
				std::cout << itemAll << std::endl;
				itemAll = item;
			}
		}
		std::cout << itemAll << std::endl;
	}
	else
	{
		std::cerr << "wrong input!" << std::endl;
		return -1;
	}
	system("pause");
	return 0;
}
