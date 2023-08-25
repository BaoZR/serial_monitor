#include "display_window.h"
#include <stdexcept>
#include <CommCtrl.h>
#include <iostream>

static const wchar_t winClass[] = L"DislpayWindow";
static const wchar_t winTitle[] = L"DislpayWindow";

static DisplayWindow* g_window = NULL;
HWND hwndTV = NULL;    // handle to tree-view control 

HTREEITEM AddItemToTree(HWND hwndTV, LPTSTR lpszItem, int nLevel)
{
    TVITEM tvi{};
    TVINSERTSTRUCT tvins{};
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
    hPrev = (HTREEITEM)SendMessageW(hwndTV, TVM_INSERTITEMW,
        0, (LPARAM)(LPTVINSERTSTRUCTW)&tvins);

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
    while (started_ == false || hwndTV == NULL)
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

bool DisplayWindow::AddItem(DeviceInfo info)
{
    if (items_.count(info.GetDbcc()) != 0)
    {
        return false;
    }

    TVITEM tvi{};
    TVINSERTSTRUCT tvins{};
    HTREEITEM handle = NULL;

    tvi.mask = TVIF_TEXT | TVIF_IMAGE
        | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    
    int len = lstrlenW((utils::to_wstring(info.GetFriendlyName()).c_str()));

    std::unique_ptr<wchar_t[]> text_holder = std::make_unique<wchar_t[]>(len + 1);//找少统帮忙？
   ::wmemcpy_s(text_holder.get(), len, utils::to_wstring(info.GetFriendlyName()).c_str(), len);
 
    tvi.pszText = text_holder.get();
    tvi.cchTextMax =len;
    tvi.lParam = (LPARAM)0;
    
    tvins.item = tvi;
    tvins.hInsertAfter = (HTREEITEM)TVI_FIRST; 
    tvins.hParent = TVI_ROOT;

    

    std::cout << "add_item" << info.GetFriendlyName() << std::endl;
    handle = (HTREEITEM)SendMessageW(hwndTV, TVM_INSERTITEMW,
        0, (LPARAM)(LPTVINSERTSTRUCTW)&tvins);
    if (handle == NULL)
    {
        return false;
    }
    if (items_.count(info.GetDbcc()) == 0)
    {
        items_[info.GetDbcc()] = handle;
    }
    utils::PrintWinError((LPWSTR)utils::to_wstring("++").c_str());

    return true;
}

bool DisplayWindow::DeleteItem(DeviceId id)
{
    bool flag = false;
    HTREEITEM handle{};
    if (items_[id] == NULL)
    {
        return false;
    }
    handle = items_[id];

    flag = SendMessageW(hwndTV, TVM_DELETEITEM, 0, (LPARAM)handle);
    
    if (flag == true)
    {
        std::cout << "delete success" << std::endl;
        items_.erase(id);
    }

    return flag;
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
        0, 0,200, 150,
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


    //HWND hwndButton = CreateWindowW(
    //    L"BUTTON",  // Predefined class; Unicode assumed 
    //    L"OK",      // Button text 
    //    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
    //    10,         // x position 
    //    10,         // y position 
    //    100,        // Button width
    //    100,        // Button height
    //    window_,     // Parent window
    //    NULL,       // No menu.
    //    (HINSTANCE)GetWindowLongPtr(window_, GWLP_HINSTANCE),
    //    NULL);      // Pointer not needed.


    InitCommonControls();

    RECT rcClient;  // dimensions of client area 


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
        200,
        100,
        window_,
        0,
        hInstance,
        NULL);

    // Initialize the image list, and add items to the control. 
    // InitTreeViewImageLists and InitTreeViewItems are application- 
    // defined functions, shown later. 



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
            //AddItemToTree(hwndTV, L"hao", 0);
            //MessageBox(0, L"eeee", L"提示", MB_OK);
        }
        break;

        default:
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    return 0;
}

