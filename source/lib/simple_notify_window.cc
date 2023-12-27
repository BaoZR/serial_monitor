#define SERIAL_MONITOR_LIB_EXPORTS
#include "simple_notify_window.h"
#include "serial_monitor_utils.h"
#include <vector>
#include <iostream>


static const wchar_t winClass[] = L"NotifyWindow";
static const wchar_t winTitle[] = L"winTitle";

static SimpleNotifyWindow* g_window = NULL;

//make sure the thread was started in the class Initialization
SimpleNotifyWindow::SimpleNotifyWindow(IDeviceChanged& device_changed)
	:window_(NULL)
	,destroy_(false)
    ,started_(false)
    ,thread_(&SimpleNotifyWindow::ThreadProc, this)
    ,device_changed_(device_changed)
{	
   
    while(started_ == false && destroy_ == false)
    {
        printf("");//这里还必须加
    }
    if (g_window != NULL)
    {
        throw std::runtime_error("Only one instance of the NotifyWindow can be created");
    }
    g_window = this;
    //puts("\nnotify window started");
}

SimpleNotifyWindow::~SimpleNotifyWindow()
{
    destroy_ = true;
    started_ = false;
    PostMessage(window_, WM_CLOSE, 0, 0);
    thread_.join();
    g_window = NULL;
    std::cout << "window delete" << std::endl;

}

HWND SimpleNotifyWindow::GetHandle() const
{
	return window_;
}



void SimpleNotifyWindow::NotifyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = SimpleNotifyWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = winClass;
    wcex.hIconSm = NULL;

    RegisterClassExW(&wcex);
}

bool SimpleNotifyWindow::InitInstance(HINSTANCE hInstance)
{
    window_ = CreateWindowW(winClass, winTitle, WS_ICONIC, 0, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if (!window_)
    {
        destroy_ = true;
        puts("CreateWindow fail\n");
        return false;
    }

    ShowWindow(window_, SW_HIDE);
    UpdateWindow(window_);

    return true;
}



LRESULT SimpleNotifyWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DEVICECHANGE:
    {
        if ((wParam == DBT_DEVICEARRIVAL) || (wParam == DBT_DEVICEREMOVECOMPLETE))
        {
            try
            {
                DEV_BROADCAST_HDR* header = reinterpret_cast<DEV_BROADCAST_HDR*>(lParam);
                if (header->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
                {
                    DEV_BROADCAST_DEVICEINTERFACE_W* devNot = reinterpret_cast<DEV_BROADCAST_DEVICEINTERFACE_W*>(lParam);
                    std::string dbcc_name = serial_monitor_utils::to_string(devNot->dbcc_name);
                    
                    if (DBT_DEVICEARRIVAL == wParam)
                    {
                        //g_NotifyWindow->deviceChanged_.InterfaceArrival(devNot->dbcc_classguid);

                        //printf("%s", devNot->dbcc_name);
                        g_window->device_changed_.InterfaceArrival(devNot->dbcc_classguid);

                        //std::cout << dbcc_name << " arrived" << std::endl;
                    }
                    else
                    {
                        g_window->device_changed_.InterfaceRemoved(serial_monitor_utils::string_lower(dbcc_name));
                        //g_NotifyWindow->deviceChanged_.InterfaceRemoved(Utilities::StringUpper(devNot->dbcc_name));
                        //std::cout << dbcc_name << " removed" << std::endl;
                    }
                }
            }
            catch (const std::runtime_error& /*ex*/)
            {
                //"ERROR: Processing WM_DEVICECHANGE failed
            }
        }
    }
    break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    break;
    default:
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}

void SimpleNotifyWindow::ThreadProc()
{
	HINSTANCE hInstance = ::GetModuleHandleW(NULL);
	NotifyRegisterClass(hInstance);
	if (!InitInstance(hInstance))//init window_
	{
		return;
	}

    this->started_ = true;

    MSG msg;
    BOOL bRet(FALSE);
    while ((bRet = GetMessageW(&msg, window_, 0, 0)) != 0)
    {
        if (destroy_)
        {
            break;
        }
        if (bRet == -1)
        {
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return;
}

