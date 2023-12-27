#pragma once
#include <Windows.h>
#include <thread>
#include <Dbt.h>
#include <stdexcept>
#include <SetupAPI.h>
#include <map>
#include "device_info.h"
#include "enum_device.h"
#include "serial_monitor_utils.h"
#include "device_changed.h"


class SimpleNotifyWindow
{
	typedef std::string DeviceId;
	public:
		SimpleNotifyWindow(IDeviceChanged& device_changed);
		~SimpleNotifyWindow();
		SimpleNotifyWindow(const SimpleNotifyWindow&) = delete;
		SimpleNotifyWindow& operator=(const SimpleNotifyWindow&) = delete;
		HWND GetHandle() const;
	private:
			void NotifyRegisterClass(HINSTANCE hInstance);
			bool InitInstance(HINSTANCE hInstance);
			//HDEVNOTIFY RegisterNotify(const GUID& guid, HANDLE hWnd);
			//void UnRegisterNotify(HDEVNOTIFY hNotyfy);
			//bool ReadDeviceInfo(const UUID& uuid, const std::string& dbcc_name,DeviceInfo& info);
	private:

		void ThreadProc();

	private:

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND window_;
		std::thread thread_;
		bool destroy_;
        bool started_;
		IDeviceChanged& device_changed_;

};

