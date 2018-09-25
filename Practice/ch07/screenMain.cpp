#include"screen.h"

int main
{
  Window_mgr window;
	Screen myScreen(5, 5, 'X');
	myScreen.move(4, 0).set('#').display(std::cout);
	std::cout << "\n";
	window.push(myScreen);
	window.get(0).display(std::cout);
	window.clear(0);
	std::cout << "\n";
	window.get(0).display(std::cout);
	std::cout << "\n";

	system("pause");
	return 0;
}
