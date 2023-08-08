#include "display_window.h"
#include <stdexcept>

static const wchar_t winClass[] = L"DislpayWindow";
static const wchar_t winTitle[] = L"DislpayWindow";

static DisplayWindow* g_window = NULL;

DisplayWindow::DisplayWindow()
    :window_(NULL)
    , started_(false)
    , thread_(&DisplayWindow::ThreadProc, this)
{
    while (started_ == false)
    {
        printf(".");
    }
    if (g_window != NULL)
    {
        throw std::runtime_error("Only one instance of the DisplayWindow can be created");
    }
    g_window = this;
    puts("\ndisplay window started");
}

DisplayWindow::~DisplayWindow()
{
    started_ = false;
    PostMessage(window_, WM_CLOSE, 0, 0);
    thread_.join();
    g_window = NULL;
}



void DisplayWindow::DisplayRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    ZeroMemory(&wcex, sizeof(wcex));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = DisplayWindow::WndProc;
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

bool DisplayWindow::InitInstance(HINSTANCE hInstance)
{
    window_ = CreateWindowW(winClass, winTitle, WS_ICONIC, 0, 0,
        CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if (!window_)
    {
        started_ = false;
        puts("CreateWindow fail\n");
        return false;
    }

    ShowWindow(window_, SW_NORMAL);
    UpdateWindow(window_);

    return true;
}


void DisplayWindow::ThreadProc()
{
    HINSTANCE hInstance = ::GetModuleHandleW(NULL);
    DisplayRegisterClass(hInstance);
    if (!InitInstance(hInstance))//init window_
    {
        return;
    }
    this->started_ = true;

    MSG msg;
    BOOL bRet(FALSE);
    while ((bRet = GetMessageW(&msg, window_, 0, 0)) != 0)
    {
        if (false == started_)
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

LRESULT DisplayWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

