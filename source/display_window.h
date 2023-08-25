#ifndef _DISPLAY_WINDOW_H_
#define _DISPLAY_WINDOW_H_
#include <Windows.h>
#include <thread>
#include <map>
#include "device_info.h"
#include "utils.h"
#include "commctrl.h"

class DisplayWindow
{
	typedef std::string DeviceId;
public:
	DisplayWindow();
	~DisplayWindow();
	DisplayWindow(const DisplayWindow&) = delete;
	DisplayWindow& operator=(const DisplayWindow&) = delete;
	bool AddItem(DeviceInfo info);
	bool DeleteItem(DeviceId Id);

private:
	bool InitInstance(HINSTANCE hInstance);
	void DisplayRegisterClass(HINSTANCE handle);
	void ThreadProc();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	HWND window_;
	std::thread thread_;
	bool started_;
	std::map<DeviceId, HTREEITEM> items_;
};



#endif // !_DISPLAY_WINDOW_H_




