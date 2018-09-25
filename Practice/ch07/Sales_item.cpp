#include "Sales_item.h"

Sales_data::Sales_data(std::istream & is)
{
	read(is, *this);
}

Sales_data & Sales_data::combine(const Sales_data &rhs)
{
	units_solds += rhs.units_solds;
	revenue += rhs.revenue;
	return *this;
}

double Sales_data::avg_price() const
{
	if (units_solds)
		return revenue / units_solds;
	else
		return 0.0;
}

Sales_data add(const Sales_data& lhs, const Sales_data& rhs)
{
	Sales_data sum = lhs;
	sum.combine(rhs);
	return sum;
}
std::ostream &print(std::ostream& os, const Sales_data& data)
{
	os << data.isbn() << " " << data.units_solds 
		<< " " << data.revenue << " " << data.avg_price();
	return os;
}
std::istream &read(std::istream& is, Sales_data& data)
{
	double price = 0;
	is >> data.bookNo >> data.units_solds >> price;
	data.revenue = price * data.units_solds;
	return is;
}
