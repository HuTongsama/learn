#pragma once
#include<map>
#include<string>
#include<iostream>
#include<fstream>
#include<algorithm>
using std::map;
using std::string;
using std::ifstream;

map<string, string> buildMap(ifstream &map_file)
{
	map<string, string> trans_map;
	string key, value;
	while (map_file >> key && std::getline(map_file, value))
	{
		if (value.size() > 1)
			trans_map[key] = value.substr(1);
		else
			throw std::runtime_error("no role for " + key);
	}

	return trans_map;
}

const string &transform(const string &s, const map<string, string> &m)
{
	auto it = m.find(s);
	if (it != m.cend())
		return it->second;
	else
		return s;
}

void word_transform(ifstream &map_file, ifstream &input)
{
	auto trans_map = buildMap(map_file);
	string text;
	string word;
	while (std::getline(input, text))
	{
		auto sIter = text.begin();
		auto fIter = text.begin();
		while (sIter != text.end())
		{
			if (fIter == text.end() || *fIter == ' ')
			{
				word.clear();
				copy(sIter, fIter, back_inserter(word));
				word = transform(word, trans_map);
				if (fIter == text.end())
				{
					std::cout << word << std::endl;
					break;
				}
				std::cout << word << ' ';
				fIter++;
				sIter = fIter;
			}
			else
				fIter++;
		}
	}
}
