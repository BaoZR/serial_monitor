#include <iostream>
#include "simple_notify_window.h"
#include "panel.h"
#include "display_window.h"


int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	int j;
	j = 5;
	Panel panel;
	panel.start();
	DisplayWindow w = DisplayWindow();
	int i;
	std::cin >> i;
	std::cout << "hello world" << std::endl;
	return 0;
}