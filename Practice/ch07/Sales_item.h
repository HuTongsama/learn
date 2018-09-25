#pragma once
#include<iostream>
#include<string>

struct Sales_data
{
	Sales_data() :bookNo(""), units_solds(0), revenue(0.0) {}
	Sales_data(const std::string &s) :bookNo(s) {}
	Sales_data(const std::string &s, unsigned n, double p) :bookNo(s), units_solds(n), revenue(n*p) {}
	Sales_data(std::istream &is);
	std::string isbn()const { return bookNo; }
	Sales_data& combine(const Sales_data&);
	double avg_price()const;

	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend std::ostream &print(std::ostream&, const Sales_data&);
	friend std::istream &read(std::istream&, Sales_data&);

private:
	std::string bookNo;
	unsigned units_solds = 0;
	double revenue = 0.0;
};

Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
