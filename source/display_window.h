#ifndef _DISPLAY_WINDOW_H_
#define _DISPLAY_WINDOW_H_
#include <Windows.h>
#include <thread>


class DisplayWindow
{
public:
	DisplayWindow();
	~DisplayWindow();
	DisplayWindow(const DisplayWindow&) = delete;
	DisplayWindow& operator=(const DisplayWindow&) = delete;

private:
	bool InitInstance(HINSTANCE hInstance);
	void DisplayRegisterClass(HINSTANCE handle);
	void ThreadProc();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	HWND window_;
	std::thread thread_;
	bool started_;
};



#endif // !_DISPLAY_WINDOW_H_




