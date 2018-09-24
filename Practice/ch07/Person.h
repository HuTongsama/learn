#pragma once
#include<iostream>
#include<string>

struct Person
{
	std::string name()const { return m_name; }
	std::string address()const { return m_address; }

	std::string m_name;
	std::string m_address;
};

std::istream& read(std::istream& is, Person &person);
std::ostream& print(std::ostream& os, const Person &person);
