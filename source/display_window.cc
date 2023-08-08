#include "display_window.h"
#include <stdexcept>
#include <CommCtrl.h>

static const wchar_t winClass[] = L"DislpayWindow";
static const wchar_t winTitle[] = L"DislpayWindow";

static DisplayWindow* g_window = NULL;

HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel)
{
    TVITEM tvi;
    TVINSERTSTRUCT tvins;
    static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST;
    static HTREEITEM hPrevRootItem = NULL;
    static HTREEITEM hPrevLev2Item = NULL;
    HTREEITEM hti;

    tvi.mask = TVIF_TEXT | TVIF_IMAGE
        | TVIF_SELECTEDIMAGE | TVIF_PARAM;

    // Set the text of the item. 
    tvi.pszText = lpszItem;
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);

    // Assume the item is not a parent item, so give it a 
    // document image. 
    //tvi.iImage = g_nDocument;
    //tvi.iSelectedImage = g_nDocument;

    // Save the heading level in the item's application-defined 
    // data area. 
    tvi.lParam = (LPARAM)nLevel;
    tvins.item = tvi;
    tvins.hInsertAfter = hPrev;

    // Set the parent item based on the specified level. 
    if (nLevel == 1)
        tvins.hParent = TVI_ROOT;
    else if (nLevel == 2)
        tvins.hParent = hPrevRootItem;
    else
        tvins.hParent = hPrevLev2Item;

    // Add the item to the tree-view control. 
    hPrev = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM,
        0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

    if (hPrev == NULL)
        return NULL;

    // Save the handle to the item. 
    if (nLevel == 1)
        hPrevRootItem = hPrev;
    else if (nLevel == 2)
        hPrevLev2Item = hPrev;

    // The new item is a child item. Give the parent item a 
    // closed folder bitmap to indicate it now has child items. 
    if (nLevel > 1)
    {
        hti = TreeView_GetParent(hwndTV, hPrev);
        tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvi.hItem = hti;
     //   tvi.iImage = g_nClosed;
    //    tvi.iSelectedImage = g_nClosed;
        TreeView_SetItem(hwndTV, &tvi);
    }

    return hPrev;
}


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
    window_ = CreateWindowW(winClass, winTitle, WS_ICONIC,
        0, 0,CW_USEDEFAULT, 0,
        NULL, NULL, hInstance, NULL);
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

    InitCommonControls();

    RECT rcClient;  // dimensions of client area 
    HWND hwndTV;    // handle to tree-view control 

    // Ensure that the common control DLL is loaded. 
    InitCommonControls();

    // Get the dimensions of the parent window's client area, and create 
    // the tree-view control. 
    GetClientRect(window_, &rcClient);
    hwndTV = CreateWindowExW(0,
        WC_TREEVIEW,
        TEXT("Tree View"),
        WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES,
        0,
        0,
        rcClient.right,
        rcClient.bottom,
        window_,
        0,
        hInstance,
        NULL);

    // Initialize the image list, and add items to the control. 
    // InitTreeViewImageLists and InitTreeViewItems are application- 
    // defined functions, shown later. 

    AddItemToTree(hwndTV,L"hao",0);
    AddItemToTree(hwndTV, L"hao", 0);
    AddItemToTree(hwndTV, L"hao", 1);

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

        case  WM_COMMAND:
        {
            MessageBox(0, L"eeee", L"提示", MB_OK);
        }
        break;

        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

