#pragma once
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<map>
#include<set>

using namespace std;

class TextQuery
{
public:
	TextQuery(ifstream &file);
	vector<string> query(const string &word)const;
	auto getMap()const { return m_mapWord; }
private:
	vector<string> m_vecStr;
	shared_ptr<map<string, set<size_t>>> m_mapWord;
};

class QueryResult
{
public:
	ostream & print(ostream &os, vector<string> vec);
	void runQueries(ifstream &infile);
private:
	shared_ptr<map<string, set<size_t>>> m_mapWord;
	string strQuery;
};
