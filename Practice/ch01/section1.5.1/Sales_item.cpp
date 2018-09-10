#include "Sales_item.h"

Sales_item::Sales_item():m_strISBN(""),m_iSold(0),m_fPrice(0.0),
m_fTotal(0.0),m_fAverage(0.0)
{
}

Sales_item operator+(const Sales_item & item1, const Sales_item & item2)
{
	Sales_item item;
	if (item1.m_strISBN != item2.m_strISBN)
		return item;
	item.m_strISBN = item1.m_strISBN;
	item.m_fTotal = item1.m_fTotal + item2.m_fTotal;
	item.m_iSold = item1.m_iSold + item2.m_iSold;
	item.m_fPrice = item.m_fTotal / item.m_iSold;
	item.m_fAverage = item.m_fPrice;
	return item;
}

std::istream & operator>>(std::istream & itemIn, Sales_item & item)
{
	itemIn >> item.m_strISBN >> item.m_iSold >> item.m_fPrice;
	item.m_fTotal = item.m_iSold*item.m_fPrice;
	item.m_fAverage = item.m_fTotal / item.m_iSold;
	return itemIn;
	// TODO: 在此处插入 return 语句
}

std::ostream & operator<<(std::ostream& itemOut, const Sales_item & item)
{
	itemOut << item.m_strISBN <<"\t" <<item.m_iSold
		<< '\t' << item.m_fTotal << '\t' << item.m_fPrice;
	return itemOut;
	// TODO: 在此处插入 return 语句
}
