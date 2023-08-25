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
	//DisplayWindow w = DisplayWindow();
	//DeviceInfo info = { "你好","223" };
	//DeviceInfo info2 = { "你好2","223" };
	//w.AddItem(info);
	//w.AddItem(info);
	//w.AddItem(info2);
	//w.DeleteItem("你好");
	//w.DeleteItem("你好");
	//w.DeleteItem("你好2");
	int i;
	std::cin >> i;
	std::cout << "hello world" << std::endl;
	return 0;
}