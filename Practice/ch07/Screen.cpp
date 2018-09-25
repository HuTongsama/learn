#include "Screen.h"

void Window_mgr::push(const Screen & s)
{
	m_screens.push_back(s);
}

Screen Window_mgr::get(ScreenIndex index) const
{
	if (index >= 0 && index < m_screens.size())
	{
		return m_screens[index];
	}
	return Screen();
}

void Window_mgr::clear(ScreenIndex index)
{
	if (index >= 0 && index < m_screens.size())
	{
		Screen &s = m_screens[index];
		s.contents = std::string(s.height*s.width, '0');
	}
}
