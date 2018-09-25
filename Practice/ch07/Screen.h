#pragma once
#include<string>
#include<vector>

class Screen;
class Window_mgr 
{
public:
	using ScreenIndex = std::vector<Screen>::size_type;
	void push(const Screen &s);
	Screen get(ScreenIndex)const;
	void clear(ScreenIndex);
private:

	std::vector<Screen> m_screens;
};

class Screen
{
public:
	using pos = std::string::size_type;
	
	Screen() = default;
	Screen(pos ht, pos wd) :height(ht), width(wd), contents(ht*wd, ' ') {}
	Screen(pos ht, pos wd, char c) :height(ht), width(wd), contents(ht*wd, c) {}

	char get()const { return contents[cursor]; }
	char get(pos row, pos col)const { return contents[row*width + col]; }
	Screen &set(const char ch)
	{
		contents[cursor] = ch;
		return *this;
	}
	Screen &move(pos row, pos col)
	{
		row = row * width;
		cursor = row + col;
		return *this;
	}
	Screen &display(std::ostream& os)
	{
		do_display(os);
		return *this;
	}
	const Screen &display(std::ostream& os)const
	{
		do_display(os);
		return *this;
	}
	friend void Window_mgr::clear(Window_mgr::ScreenIndex);
private:
	void do_display(std::ostream& os)const
	{
		os << contents;
	}
private:
	pos cursor = 0;
	pos height = 0;
	pos width = 0;
	std::string contents;
};
