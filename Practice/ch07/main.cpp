#include"Sales_item.h"

int main()
{
	Sales_data total;
	if (read(std::cin,total))
	{
		Sales_data trans;
		while (read(std::cin,trans))
		{
			if (total.bookNo == trans.bookNo)
			{
				total.combine(trans);
			}
			else
			{
				print(std::cout, total) << std::endl;
				total = trans;
			}
		}
		print(std::cout, total) << std::endl;
	}
	else
	{
		std::cerr << "No data?!" << std::endl;
		return -1;
	}
	system("pause");
	return 0;
}
