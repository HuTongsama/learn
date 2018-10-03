#include "myQuery.h"

TextQuery::TextQuery(ifstream & file)
{
	string str, word;
	m_mapWord = make_shared<map<string, set<size_t>>>();
	while (getline(file, str))
	{
		istringstream stream(str);
		m_vecStr.push_back(str);
		while (stream >> word)
		{
			for (auto it = word.begin(); it != word.end(); it++)
			{
				if (ispunct(*it))
					it = word.erase(it);
				if (it == word.end())
					break;
			}
			
			(*m_mapWord)[word].insert(m_vecStr.size() - 1);
		}
	}
}

vector<string> TextQuery::query(const string & word) const
{
	vector<string>res;
	auto mapIter = m_mapWord->find(word);
	if (mapIter != m_mapWord->end())
		for (const auto ele : mapIter->second)
			res.push_back(m_vecStr[ele]);
	return res;
}

ostream & QueryResult::print(ostream & os, vector<string> vec)
{
	os << strQuery << " occurs " << vec.size() << " times." << endl;
	if (vec.size() == 0)
		return os;
	auto iterMap = m_mapWord->find(strQuery);
	auto iterVec = vec.begin();
	for (auto ele : iterMap->second)
	{
		os << "(line " << ele + 1 << ") " << *iterVec << endl;
		iterVec++;
	}
	return os;
}

void QueryResult::runQueries(ifstream & infile)
{
	TextQuery tq(infile);
	m_mapWord = tq.getMap();
	while (true) 
	{
		cout << "enter word to look for, or q to quit: ";
		string s;
		if (!(cin >> s) || s == "q") break;
		strQuery = s;
		print(cout, tq.query(s)) << endl;
	}
}
