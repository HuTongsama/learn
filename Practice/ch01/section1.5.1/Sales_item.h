#pragma once
#include<iostream>
#include<string>
class Sales_item
{
public:
	Sales_item();
	friend Sales_item operator+(const Sales_item& item1,const Sales_item& item2);
	friend std::istream& operator>>(std::istream& itemIn, Sales_item& item);
	friend std::ostream& operator<<(std::ostream& itemOut, const Sales_item& item);
private:
	std::string m_strISBN;
	int m_iSold;
	float m_fPrice;
	float m_fTotal;
	float m_fAverage;

};
