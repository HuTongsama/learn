#include "Person.h"

std::istream & read(std::istream & is, Person & person)
{
	is >> person.m_name;
	return is;
}

std::ostream & print(std::ostream & os, const Person & person)
{
	os << person.name();
	return os;
}
