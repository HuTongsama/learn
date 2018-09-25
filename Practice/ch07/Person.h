#pragma once
#include<iostream>
#include<string>

struct Person
{
	Person() = default;
	Person(const std::string &n, const std::string &a) :m_name(n), m_address(a) {}
	
	std::string name()const { return m_name; }
	std::string address()const { return m_address; }

	std::string m_name;
	std::string m_address;
};

std::istream& read(std::istream& is, Person &person);
std::ostream& print(std::ostream& os, const Person &person);
