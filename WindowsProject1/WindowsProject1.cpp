// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"

// Added libraries:
#include <iostream>     // debugging purposes in console
#include <vector>       // dynamic array
#include <sstream>      // stringstream

#define MAX_LOADSTRING 100

// Capturing styles
#define CW_MAX_WIDTH -1
#define CW_MAX_HEIGHT -1

// Results for capturing extension's window
#define CWE_COMPLETE 1
#define CWE_CONTINUE 2
#define CWE_TERMINATE 3

#ifdef _DEBUG
#define TELEGRAM_CLASSNAME          L"WinUIDesktopWin32WindowClass"
#define TELEGRAM_WINDOWNAME         L"Telegram.png"
#define TELEGRAM_POPUP_CLASSNAME    L"WinUIDesktopWin32WindowClass"
#define TELEGRAM_POPUP_WINDOWNAME   L"Popup.png"
#define LARK_CLASSNAME              L"Chrome_WidgetWin_1"
#define LARK_WINDOWNAME             L"Lark"
#define EXTENSION_CLASSNAME         L"Chrome_WidgetWin_1"
#define EXTENSION_WINDOWNAME        L"Tracking - Google Chrome"
#else
#define TELEGRAM_CLASSNAME          L"Qt51517QWindowIcon"
#define TELEGRAM_WINDOWNAME         NULL
#define TELEGRAM_POPUP_CLASSNAME    L"Qt51517QWindowPopupSaveBits"
#define TELEGRAM_POPUP_WINDOWNAME   L"TelegramDesktop"
#define LARK_CLASSNAME              L"Chrome_WidgetWin_1"
#define LARK_WINDOWNAME             L"Lark"
#define EXTENSION_CLASSNAME         L"Chrome_WidgetWin_1"
#define EXTENSION_WINDOWNAME        L"Tracking"
#endif

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hBtnAuto;                                  // Button's window handle
HWND hBtnTg;                                    // Button's window handle for telegram
BOOL isCapturing = FALSE;                       // On/Off switch
BOOL isInstant = FALSE;                         // Trigger capture instantly
BOOL isOnce = FALSE;                            // Trigger capture once
BOOL isRunning = TRUE;                          // Entire window running status
BOOL isTgEnabled = FALSE;

// Enum for IDs of Window elements
enum {
    ID_BUTTON_AUTO,
    ID_BUTTON_TG
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Added functions:
BOOL CALLBACK   PrintWindowInfo(HWND, LPARAM);                  // EnumWindows(PrintWindowInfo, NULL);
BOOL            IsAltTabWindow(HWND);
int             GetAbsoluteRect(HWND, RECT*);
int             CaptureAnImage(HWND);                           // Sample from Microsoft
BOOL            CaptureWindow(HWND, int, int, int, int, BYTE*); //
int             AnalyzeWindowExt(HWND);
int             AnalyzeWindowTg(HWND, int, int);

// Window functions:
int ToggleButton(HWND);

// Input functions:
int CopyToClipboard(wchar_t*);
int SendInputByBlocking(_In_ UINT cInputs, _In_reads_(cInputs) LPINPUT pInputs, _In_ int cbSize, BOOL cd);
int SendInputByBlocking(_In_ UINT cInputs, _In_reads_(cInputs) LPINPUT pInputs, _In_ int cbSize);
int Click(int, int);
int Click(HWND, int, int);
int RClick(int, int);
int RClick(HWND, int, int);
int Drag(int, int, int, int);
int PressKey(HWND, WORD, BOOL);
int PressKey(HWND, WORD);
int HoldKey(HWND, WORD);
int ReleaseKey(HWND, WORD);
int Paste(HWND);
int Enter(HWND);
int Tab(HWND);
int Esc(HWND);
int ArrowKeyDown(HWND);
int Backspace(HWND);

// Macro functions:
int SearchTgGroup(HWND);
int SearchTgMessage(HWND);
int PasteTgMessage(HWND);
int CancelTgReply(HWND);
int GetTgContextMenuY(HWND, int);
int SearchLarkGroup(HWND);
int SearchLarkMessage(HWND);
int PasteLarkMessage(HWND);

int main()
{
    HWND hWndExt;
    HWND hWndTg;
    HWND hWndLark;

    hWndExt = FindWindow(EXTENSION_CLASSNAME, EXTENSION_WINDOWNAME);
    hWndTg = FindWindow(TELEGRAM_CLASSNAME, TELEGRAM_WINDOWNAME);
    hWndLark = FindWindow(LARK_CLASSNAME, LARK_WINDOWNAME);

#ifdef _DEBUG
    // DEBUG

    //int result = SearchLarkGroup(hWndLark);

    //if (result == 0)
    //    PasteLarkMessage(hWndLark);

    //return CWE_TERMINATE;

    int result = SearchLarkGroup(hWndTg);

    //if (result == 0)
    //    AnalyzeWindowTg(hWndTg, 0, 0);

    return CWE_TERMINATE;
#endif

    if (!hWndExt)
    {
        MessageBox(NULL, L"Extension not found", L"Program unable to proceed", MB_ICONERROR);

        return CWE_TERMINATE;
    }

    if (!hWndTg)
    {
        MessageBox(NULL, L"Telegram not found", L"Program unable to proceed", MB_ICONERROR);

        return CWE_TERMINATE;
    }

    if (!hWndLark)
    {
        MessageBox(NULL, L"Lark not found", L"Program unable to proceed", MB_ICONERROR);

        return CWE_TERMINATE;
    }

    return AnalyzeWindowExt(hWndExt);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
#ifndef _DEBUG
    HW_PROFILE_INFO hwProfileInfo;

    if (GetCurrentHwProfile(&hwProfileInfo) == NULL)
        return FALSE;

    //MessageBox(NULL, hwProfileInfo.szHwProfileGuid, L"Hardware Profile Guid ID", MB_OK);
    //CopyToClipboard(hwProfileInfo.szHwProfileGuid);

    if (wcscmp(hwProfileInfo.szHwProfileGuid, L"{da53762f-f986-11ef-aa90-806e6f6e6963}") != 0
        && wcscmp(hwProfileInfo.szHwProfileGuid, L"{5fe8ebce-18bf-11ea-a8ee-806e6f6e6963}") != 0
        && wcscmp(hwProfileInfo.szHwProfileGuid, L"{ef656140-d133-11ee-8431-806e6f6e6963}") != 0)
        return FALSE;
#endif

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg{};

    int result = CWE_COMPLETE;
    ULONGLONG lastTime = GetTickCount64();

    // Main message loop:
    while (isRunning)
    {
        if (!isCapturing && result != CWE_CONTINUE)
        {
            lastTime = GetTickCount64();
        }

        if (isOnce)
        {
            isOnce = FALSE;

            result = CWE_TERMINATE;
        }

        if (isInstant || ((isCapturing || result == CWE_CONTINUE) && GetTickCount64() - lastTime > 10000))
        {
            if (isInstant)
            {
                isInstant = FALSE;
            }

            result = main();

            switch (result)
            {
            case CWE_COMPLETE:
                lastTime = GetTickCount64() - 9000; // Cooldown 1 second
                break;
            case CWE_CONTINUE:
                lastTime = GetTickCount64() - 8500; // Cooldown 1.5 seconds
                break;
            case CWE_TERMINATE:
                if (isCapturing)
                {
                    ToggleButton(msg.hwnd);
                }
                break;
            default:
                break;
            }
        }

        while (!isInstant && ((!isCapturing && result != CWE_CONTINUE) || GetTickCount64() - lastTime <= 10000) && PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                isRunning = FALSE;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 480, 240, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            RegisterHotKey(hWnd, 100, 0, VK_F1);
            RegisterHotKey(hWnd, 100, 0, VK_F2);
            RegisterHotKey(hWnd, 100, 0, VK_F3);
            RegisterHotKey(hWnd, 100, 0, VK_F4);

            hBtnAuto = CreateWindow(L"BUTTON", L"自动",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                10, 10,
                52, 32,
                hWnd, (HMENU)ID_BUTTON_AUTO, hInst, NULL);

            hBtnTg = CreateWindow(L"BUTTON", L"Telegram",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE,
                72, 10,
                92, 32,
                hWnd, (HMENU)ID_BUTTON_TG, hInst, NULL);
        }
        break;
    case WM_HOTKEY:
        {
            SHORT ksF1 = GetAsyncKeyState(VK_F1);
            SHORT ksF2 = GetAsyncKeyState(VK_F2);
            SHORT ksF3 = GetAsyncKeyState(VK_F3);
            SHORT ksF4 = GetAsyncKeyState(VK_F4);

            if ((ksF1 << 15) & ksF1)
            {
                //::_tsystem(L"taskkill /F /T /IM WindowsProject1.exe");
            }
            else if (ksF2 << 15 & ksF2)
            {
                isInstant = TRUE;
            }
            else if (ksF3 << 15 & ksF3)
            {
                ToggleButton(hWnd);
            }
            else if (ksF4 << 15 & ksF4)
            {
                isOnce = TRUE;
            }
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_BUTTON_AUTO:
                {
                    isCapturing = IsDlgButtonChecked(hWnd, ID_BUTTON_AUTO);

                    SendMessage(hBtnAuto, BM_SETSTATE, (WPARAM)isCapturing, NULL);
                }
                break;
            case ID_BUTTON_TG:
                {
                    isTgEnabled = IsDlgButtonChecked(hWnd, ID_BUTTON_TG);
                }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//
//  FUNCTION: PrintWindowInfo(HWND, LPARAM)
//
//  PURPOSE: Callback function for EnumWindow
//
BOOL CALLBACK PrintWindowInfo(HWND hWnd, LPARAM lParam) {
    WCHAR windowTitle[MAX_LOADSTRING];

    GetWindowTextW(hWnd, windowTitle, MAX_LOADSTRING);

    WCHAR windowClassName[MAX_LOADSTRING];

    GetClassNameW(hWnd, windowClassName, MAX_LOADSTRING);

    int length = GetWindowTextLength(hWnd);
    std::wstring title = (&windowTitle[0]);

    if (!IsWindowVisible(hWnd) || length == 0 || title == L"Program Manager") {
        return TRUE;
    }

    MessageBox(NULL, windowClassName, windowTitle, MB_OK);
    
    std::string str = std::string(title.begin(), title.end());
    const char* ch = &(str.at(0));

    std::wcout << windowTitle << std::endl << windowClassName << std::endl;

    RECT rect;

    GetWindowRect(hWnd, &rect);

    std::wcout << L"Top left    : (" << rect.left << L", " << rect.top << L")" << std::endl;
    std::wcout << L"Bottom right: (" << rect.right << L", " << rect.bottom << L")" << std::endl;

    //MessageBox(NULL, windowClassName, windowTitle, MB_OK);

    DWORD* affinity = new DWORD[1];

    GetWindowDisplayAffinity(hWnd, affinity);

    std::wcout << *affinity << std::endl;

    WINDOWINFO wi;

    GetWindowInfo(hWnd, &wi);

    std::vector<DWORD> dwStyles = {
        WS_BORDER,
        WS_CAPTION,
        WS_CHILD,
        WS_CHILDWINDOW,
        WS_CLIPCHILDREN,
        WS_CLIPSIBLINGS,
        WS_DISABLED,
        WS_DLGFRAME,
        WS_GROUP,
        WS_HSCROLL,
        WS_ICONIC,
        WS_MAXIMIZE,
        WS_MAXIMIZEBOX,
        WS_MINIMIZE,
        WS_MINIMIZEBOX,
        WS_OVERLAPPED,
        WS_OVERLAPPEDWINDOW,
        WS_POPUP,
        WS_POPUPWINDOW,
        WS_SIZEBOX,
        WS_SYSMENU,
        WS_TABSTOP,
        WS_THICKFRAME,
        WS_TILED,
        WS_TILEDWINDOW,
        WS_VISIBLE,
        WS_VSCROLL
    };

    std::vector<std::string> strStyles = {
        "WS_BORDER",
        "WS_CAPTION",
        "WS_CHILD",
        "WS_CHILDWINDOW",
        "WS_CLIPCHILDREN",
        "WS_CLIPSIBLINGS",
        "WS_DISABLED",
        "WS_DLGFRAME",
        "WS_GROUP",
        "WS_HSCROLL",
        "WS_ICONIC",
        "WS_MAXIMIZE",
        "WS_MAXIMIZEBOX",
        "WS_MINIMIZE",
        "WS_MINIMIZEBOX",
        "WS_OVERLAPPED",
        "WS_OVERLAPPEDWINDOW",
        "WS_POPUP",
        "WS_POPUPWINDOW",
        "WS_SIZEBOX",
        "WS_SYSMENU",
        "WS_TABSTOP",
        "WS_THICKFRAME",
        "WS_TILED",
        "WS_TILEDWINDOW",
        "WS_VISIBLE",
        "WS_VSCROLL"
    };

    std::wcout << L"Window Styles: " << std::endl;
    for (int i = 0; i < dwStyles.size(); i++) {
        if ((wi.dwStyle & dwStyles.at(i)) == dwStyles.at(i)) {
            std::cout << strStyles.at(i) << ", ";
        }
    }
    std::cout << std::endl;

    std::vector<DWORD> exStyles = {
        WS_EX_ACCEPTFILES,
        WS_EX_APPWINDOW,
        WS_EX_CLIENTEDGE,
        WS_EX_COMPOSITED,
        WS_EX_CONTEXTHELP,
        WS_EX_CONTROLPARENT,
        WS_EX_DLGMODALFRAME,
        WS_EX_LAYERED,
        WS_EX_LAYOUTRTL,
        WS_EX_LEFT,
        WS_EX_LEFTSCROLLBAR,
        WS_EX_LTRREADING,
        WS_EX_MDICHILD,
        WS_EX_NOACTIVATE,
        WS_EX_NOINHERITLAYOUT,
        WS_EX_NOPARENTNOTIFY,
        WS_EX_NOREDIRECTIONBITMAP,
        WS_EX_OVERLAPPEDWINDOW,
        WS_EX_PALETTEWINDOW,
        WS_EX_RIGHT,
        WS_EX_RIGHTSCROLLBAR,
        WS_EX_RTLREADING,
        WS_EX_STATICEDGE,
        WS_EX_TOOLWINDOW,
        WS_EX_TOPMOST,
        WS_EX_TRANSPARENT,
        WS_EX_WINDOWEDGE
    };

    std::vector<std::string> strExStyles = {
        "WS_EX_ACCEPTFILES",
        "WS_EX_APPWINDOW",
        "WS_EX_CLIENTEDGE",
        "WS_EX_COMPOSITED",
        "WS_EX_CONTEXTHELP",
        "WS_EX_CONTROLPARENT",
        "WS_EX_DLGMODALFRAME",
        "WS_EX_LAYERED",
        "WS_EX_LAYOUTRTL",
        "WS_EX_LEFT",
        "WS_EX_LEFTSCROLLBAR",
        "WS_EX_LTRREADING",
        "WS_EX_MDICHILD",
        "WS_EX_NOACTIVATE",
        "WS_EX_NOINHERITLAYOUT",
        "WS_EX_NOPARENTNOTIFY",
        "WS_EX_NOREDIRECTIONBITMAP",
        "WS_EX_OVERLAPPEDWINDOW",
        "WS_EX_PALETTEWINDOW",
        "WS_EX_RIGHT",
        "WS_EX_RIGHTSCROLLBAR",
        "WS_EX_RTLREADING",
        "WS_EX_STATICEDGE",
        "WS_EX_TOOLWINDOW",
        "WS_EX_TOPMOST",
        "WS_EX_TRANSPARENT",
        "WS_EX_WINDOWEDGE"
    };

    std::wcout << L"Window ExStyles: " << std::endl;
    for (int i = 0; i < exStyles.size(); i++) {
        if ((wi.dwExStyle & exStyles.at(i)) == exStyles.at(i)) {
            std::cout << strExStyles.at(i) << ", ";
        }
    }
    std::cout << std::endl;

    //CaptureScreen(hWnd);

    std::cout << std::endl;

    return TRUE;
}

//
//  FUNCTION: IsAltTabWindow(HWND)
//
//  PURPOSE: Check if the window exists in taskbar
//
BOOL IsAltTabWindow(HWND hwnd)
{
    // Start at the root owner
    HWND hwndWalk = GetAncestor(hwnd, GA_ROOTOWNER);

    // See if we are the last active visible popup
    HWND hwndTry;
    while ((hwndTry = GetLastActivePopup(hwndWalk)) != /* hwndTry */ hwndWalk) {
        if (IsWindowVisible(hwndTry)) break;
        hwndWalk = hwndTry;
    }
    return hwndWalk == hwnd;
}

int GetAbsoluteRect(HWND hWnd, RECT* rc) {
    RECT rcClient, rcWindow;

    GetClientRect(hWnd, &rcClient);
    GetWindowRect(hWnd, &rcWindow);

    int x = rcWindow.right - rcWindow.left - rcClient.right;
    int y = rcWindow.bottom - rcWindow.top - rcClient.bottom;

    rc->left = rcWindow.left + x / 2;
    rc->right = rcWindow.right - x / 2;
    rc->top = rcWindow.top + 2;
    rc->bottom = rcWindow.bottom - y;

    return 0;
}

// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

//
//   FUNCTION: CaptureAnImage(HWND hWnd)
//
//   PURPOSE: Captures a screenshot into a window ,and then saves it in a .bmp file.
//
//   COMMENTS: 
//
//      Note: This function attempts to create a file called captureqwsx.bmp 
//        

int CaptureAnImage(HWND hWnd)
{
    // Debugging
    HWND hWndExt = FindWindow(EXTENSION_CLASSNAME, EXTENSION_WINDOWNAME);
    HWND hWndTg = FindWindow(TELEGRAM_CLASSNAME, TELEGRAM_WINDOWNAME);
    HWND hWndCal = FindWindow(NULL, L"Calculator");

    HDC hdcScreen;
    HDC hdcWindow;
    HDC hdcMemDC = NULL;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeofDIB = 0;
    HANDLE hFile = NULL;
    char* lpbitmap = NULL;
    HANDLE hDIB = NULL;
    DWORD dwBmpSize = 0;

    // Retrieve the handle to a display device context for the client 
    // area of the window. 
    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcWindow);

    if (!hdcMemDC)
    {
        MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
        goto done;
    }

    // Get the client area for size calculation.
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    // This is the best stretch mode.
    SetStretchBltMode(hdcWindow, HALFTONE);

    // The source DC is the entire screen, and the destination DC is the current window (HWND).
    if (!StretchBlt(hdcWindow,
        0, 0,
        rcClient.right, rcClient.bottom,
        hdcScreen,
        0, 0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        SRCCOPY))
    {
        MessageBox(hWnd, L"StretchBlt has failed", L"Failed", MB_OK);
        goto done;
    }

    // Create a compatible bitmap from the Window DC.
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

    if (!hbmScreen)
    {
        MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
        goto done;
    }

    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);

    // Bit block transfer into our compatible memory DC.
    if (!BitBlt(hdcMemDC,
        0, 0,
        rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
        hdcWindow,
        0, 0,
        SRCCOPY))
    {
        MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
        goto done;
    }

    goto done;

    // Get the BITMAP from the HBITMAP.
    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

    BITMAPFILEHEADER   bmfHeader;
    BITMAPINFOHEADER   bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc.
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpbitmap = (char*)GlobalLock(hDIB);

    // Gets the "bits" from the bitmap, and copies them into a buffer 
    // that's pointed to by lpbitmap.
    GetDIBits(hdcWindow, hbmScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpbitmap,
        (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // A file is created, this is where we will save the screen capture.
    hFile = CreateFile(L"captureqwsx.bmp",
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);

    // Add the size of the headers to the size of the bitmap to get the total file size.
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Offset to where the actual bitmap bits start.
    bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    // Size of the file.
    bmfHeader.bfSize = dwSizeofDIB;

    // bfType must always be BM for Bitmaps.
    bmfHeader.bfType = 0x4D42; // BM.

    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

    // Unlock and Free the DIB from the heap.
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);

    // Close the handle for the file that was created.
    CloseHandle(hFile);

    // Clean up.
done:
    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);

    return 0;
}

BOOL CaptureWindow(HWND hWnd, int x, int y, int cx, int cy, BYTE* pBytes) {
    HDC hdcDkt;
    HDC hdcMem;
    RECT rcAbs;
    RECT rcWnd;
    RECT rcScr;
    int cxAbs;
    int cyAbs;
    int xTemp;
    int yTemp;
    BITMAPINFO bmi;
    HBITMAP hbm;
    HBITMAP hDDB;
    HBITMAP hDIB;
    HGDIOBJ oldhDDB;
    HWND fgHwnd;
    HWND prevHwnd;
    UINT uFlags;
    BOOL success = TRUE;

    // Retrives all device contexts
    hdcDkt = GetDC(NULL);
    hdcMem = CreateCompatibleDC(hdcDkt);

    // Gets all sizes
    GetAbsoluteRect(hWnd, &rcAbs);
    GetWindowRect(hWnd, &rcWnd);
    SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScr, 0);
    
    cxAbs = rcAbs.right - rcAbs.left;
    cyAbs = rcAbs.bottom - rcAbs.top;

    // Sets maximum possible width and height
    if (cy == CW_MAX_HEIGHT || cy > cyAbs - y)
    {
        cy = cyAbs - y;
    }
    if (cx == CW_MAX_WIDTH || cx > cxAbs - x)
    {
        cx = cxAbs - x;
    }

    // Sets x & y to ensure windows fits within the screen for capturing
    xTemp = (rcAbs.left + x) >= rcScr.left ? rcAbs.left : -x;
    xTemp = (xTemp + x + cx) <= rcScr.right ? xTemp : rcScr.right - x - cx; // Derived from: xTemp - (xTemp + x + cx - rcScr.right)
    yTemp = (rcAbs.top + y) >= rcScr.top ? rcAbs.top : -y;
    yTemp = (yTemp + y + cy) <= rcScr.bottom ? yTemp : rcScr.bottom - y -  cy; // Derived from: yTemp - (yTemp + y + cx - rcScr.bottom)

    // Initializes Bitmap for Device-Independent Bitmap
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = cx;
    bmi.bmiHeader.biHeight = -cy;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // 4 Bytes including tranparency
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = cxAbs * 4 * cyAbs;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    // Creates Device-Dependent & Device-Independent Bitmaps
    hDDB = CreateCompatibleBitmap(hdcDkt, cx, cy); // Clipboard purposes
    hbm = hDDB;

    // Replaced with GetDIBits()
    //      hDIB = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, (void**)(&pBytes), NULL, NULL); // Access-pixel purposes
    //      hDIB = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, 0, NULL, NULL);
    
    oldhDDB = SelectObject(hdcMem, hbm);

    // Snapshot active window and previous window of target window
    fgHwnd = GetForegroundWindow();

    if (fgHwnd == hWnd)
    {
        prevHwnd = HWND_NOTOPMOST;
    }
    else
    {
        prevHwnd = GetWindow(hWnd, GW_HWNDPREV);

        while (prevHwnd && !IsWindowVisible(prevHwnd) && !IsAltTabWindow(prevHwnd) && GetWindowTextLength(prevHwnd) == 0)
        {
            prevHwnd = GetWindow(prevHwnd, GW_HWNDPREV);
        }
    }

    if (GetWindowLong(hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST) // Target window already at top most
    {
        uFlags = SWP_NOSIZE | SWP_NOZORDER;
    }
    else
    {
        uFlags = SWP_NOSIZE;
    }

    // Brings target window to top most
    SetWindowPos(hWnd, HWND_TOPMOST,
        xTemp + rcWnd.left - rcAbs.left, // Offset the border
        yTemp + rcWnd.top - rcAbs.top,
        0, 0,
        uFlags);
    //SetForegroundWindow(hWnd); // Mandatory to trigger SetWindowPos immediately to retore the window
    
    // Transfers bitmap
    BitBlt(hdcMem, 0, 0, cx, cy, hdcDkt, xTemp + x, yTemp + y, SRCCOPY);
    GetDIBits(hdcMem, hbm, 0, cy, &pBytes[0], &bmi, DIB_RGB_COLORS);

    // DEBUG: Copies to clipboard
    #ifdef _DEBUG
        OpenClipboard(NULL);
        EmptyClipboard();
        SetClipboardData(CF_BITMAP, hDDB);
        CloseClipboard();
    #endif

    // Restores the target window
    if (!SetWindowPos(hWnd, HWND_NOTOPMOST, rcWnd.left, rcWnd.top, 0, 0, SWP_NOSIZE)
        || !SetWindowPos(hWnd, prevHwnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE))
    {
        success = FALSE;

        MessageBox(NULL, L"SetWindowPos failed", L"Failed", MB_ICONERROR);

        // DEBUG: Check previous window
        PrintWindowInfo(prevHwnd, NULL);
    }

    // Restore active window;
    SetForegroundWindow(fgHwnd);

    // Free memory
    SelectObject(hdcMem, oldhDDB);
    DeleteDC(hdcMem);
    DeleteObject(hbm);
    ReleaseDC(hWnd, hdcDkt);

    return success;
}

//
//  FUNCTION: AnalyzeWindowExt(HWND)
//
//  PURPOSE: Analyze pixels on extension's window
//
int AnalyzeWindowExt(HWND hWnd)
{
    HWND hWndTg = FindWindow(TELEGRAM_CLASSNAME, TELEGRAM_WINDOWNAME);
    HWND hWndLark = FindWindow(LARK_CLASSNAME, LARK_WINDOWNAME);
    // Window
    RECT rcAbs;
    int ox;
    int oy;
    int cx;
    int cy;
    // Pixels
    int oy2 = 1;
    int x = 0;
    int y = 0;
    int a = -1;
    int r = -1;
    int g = -1;
    int b = -1;
    int result = CWE_COMPLETE;
    int success = TRUE;

    GetAbsoluteRect(hWnd, &rcAbs);

    // DEBUG: Replace orignal position with sample
    ox = 22;
    oy = 197;
    cx = 1;
    cy = rcAbs.bottom - (rcAbs.top + oy);

    // Check tracking's color indicators
    ox = 17;
    oy = 24;
    cx = 1;
    cy = rcAbs.bottom - (rcAbs.top + oy);

    BYTE* pBytes = new BYTE[4 * cx * cy];
    BYTE* pBytesTemp = new BYTE[4];

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    for (int i = 0; i < cx * cy * 4; i += 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        if (r == 255 && g == 0 && b == 1)
        {
            Click(hWnd, x, y + oy2); // Copies macro's input

            char input[100];
            wchar_t clipboard[100];

            OpenClipboard(NULL);
            strcpy_s(input, (char*)GetClipboardData(CF_TEXT));
            wcscpy_s(clipboard, (wchar_t*)GetClipboardData(CF_UNICODETEXT));
            CloseClipboard();

            std::stringstream ss(input);
            std::vector<std::string> args;

            for (std::string line; std::getline(ss, line, '\n'); ) {
                line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

                args.push_back(line);
            }

            if ("drag" == args.at(0))
            {
                float x1 = atof(args.at(1).c_str());
                float y1 = atof(args.at(2).c_str());
                float x2 = atof(args.at(3).c_str());
                float y2 = atof(args.at(4).c_str());

                Drag(x1, y1, x2, y2);

                success = TRUE;
            }
            else
            {
                MessageBox(NULL, clipboard, L"Macro Error", MB_OK);

                success = FALSE;
            }

            Click(hWnd, x, y + oy2); // Informs extension macro completed

            result = success ? CWE_CONTINUE : CWE_TERMINATE;
            break;
        }
        else if (isTgEnabled && r == 0 && g == 70 && b == 255)
        {
            result = AnalyzeWindowTg(hWndTg, x, y);
            break;
        }
        else if ((r == 255 || r == 200) && g == 0 && b == 0)
        {
            HWND hWndApp = (r == 255) ? hWndTg : hWndLark;

            Click(hWnd, x, y + oy2); // Copies group name

            if (r == 255)
                SearchTgGroup(hWndApp);
            else
                SearchLarkGroup(hWndApp);

            Click(hWnd, x, y + oy2); // Copies transaction id

            if (r == 255)
                PasteTgMessage(hWndApp);
            else
                PasteLarkMessage(hWndApp);

            while (true)
            {
                Sleep(150);

                CaptureWindow(hWnd, x, y, 1, 1, &pBytesTemp[0]);
                r = pBytesTemp[2];
                g = pBytesTemp[1];
                b = pBytesTemp[0];

                if ((r == 255 || r == 200) && g == 0 && b == 0)
                {
                    Click(hWnd, x, y + oy2); // Copies attachment
                    Paste(hWndApp);
                }
                else // Finished copying
                {
                    Sleep(500); // Wait for attachments to load
                    Enter(hWndApp);
                    break;
                }
            }

            if ((r == 255 || r == 200) && g == 165 && b == 0)
            {
                Click(hWnd, x, y + oy2); // Trigger extension's submit message

                while (success && (r == 255 || r == 200) && g == 165 && b == 0) // Wait for submit message sent successfully
                {
                    Sleep(150);

                    success = CaptureWindow(hWnd, x, y, 1, 1, &pBytesTemp[0]);
                    r = pBytesTemp[2];
                    g = pBytesTemp[1];
                    b = pBytesTemp[0];
                }
            }

            result = success ? CWE_CONTINUE : CWE_TERMINATE;
            break;
        }
        else if (r == 0 && g == 165 && b == 0)
        {
            Click(hWnd, x, y + oy2); // Trigger extension's complete message

            while (success && r == 0 && g == 165 && b == 0) // Wait for ccomplete message sent successfully
            {
                Sleep(150);

                success = CaptureWindow(hWnd, x, y, 1, 1, &pBytesTemp[0]);
                r = pBytesTemp[2];
                g = pBytesTemp[1];
                b = pBytesTemp[0];
            }

            result = success ? CWE_CONTINUE : CWE_TERMINATE;
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    return result;
}

//
//  FUNCTION: AnalyzeWindowTg(HWND, int, int)
//
//  PURPOSE: Analyze pixels on Telegram's window
//
int AnalyzeWindowTg(HWND hWnd, int X, int Y)
{
    HWND hWndExt = FindWindow(EXTENSION_CLASSNAME, EXTENSION_WINDOWNAME);
    // Window
    RECT rcAbs;
    RECT rcChat;
    RECT rcMsg{};
    BOOL isLoading = FALSE;
    wchar_t clipboard[100];
    int sides[4][4];
    int ox;
    int oy;
    int cx;
    int cy;
    // Pixels
    int oy2 = 1;
    int x = 0;
    int y = 0;
    int a = -1;
    int r = -1;
    int g = -1;
    int b = -1;
    int result = CWE_CONTINUE;
    int success = TRUE;

    GetAbsoluteRect(hWnd, &rcAbs);

    BYTE* pBytes = new BYTE[4 * (rcAbs.right - rcAbs.left) * (rcAbs.bottom - rcAbs.top)];
    BYTE* pBytesTemp = new BYTE[4];

#ifndef _DEBUG
    Click(hWndExt, X, Y + 1); // Copies transaction id

    if (SearchTgMessage(hWnd) == 1) // Search list exceeded the canvas
    {
        wcscpy_s(clipboard, L"-1");
        CopyToClipboard(clipboard);

        goto ending;
    }
#endif

    // DEBUG: Replace orignal position with sample
    ox = 0;
    oy = (rcAbs.bottom - rcAbs.top) - 5 - 72;
    cx = rcAbs.right - (rcAbs.left + ox) - 5;
    cy = 1;

    // Finds chat channel's left end
#ifndef _DEBUG
    ox = 0;
    oy = (rcAbs.bottom - rcAbs.top) - 5;
    cx = rcAbs.right - (rcAbs.left + ox) - 5;
    cy = 1;
#endif

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    for (int i = (cx - 1) * 4; i >= ox * 4; i -= 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        // Chat bar's rgb = 23, 33, 43
        if (r == 23 && g == 33 && b == 43)
        {
            // Continues
        }
        else
        {
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    rcChat.left = rcAbs.left + x + 1;
    rcChat.right = rcAbs.right - 86;

    // DEBUG: Replace orignal position with sample
    ox = rcChat.left - rcAbs.left;
    oy = 0 + 72;
    cx = 1;
    cy = (rcAbs.bottom - rcAbs.top) / 2;

    // Finds chat channel's top end
#ifndef _DEBUG
    ox = rcChat.left - rcAbs.left;
    oy = 0;
    cx = 1;
    cy = (rcAbs.bottom - rcAbs.top) / 2;
#endif

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    for (int i = 0; i < cx * cy * 4; i += 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        // Chat channel background's rgb
        if (r == 14 && g == 22 && b == 33)
        {
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    // DEBUG: Replace orignal position with sample
    rcChat.top = rcAbs.top + y;
    rcChat.bottom = (rcAbs.bottom - 72) - 48; // Omits chat bar

    // Defines chat channel's right end
#ifndef _DEBUG
    rcChat.top = rcAbs.top + y;
    rcChat.bottom = rcAbs.bottom - 48; // Omits chat bar
#endif

find_highlighted_msg:

    // Waits highlighted message fade in
    ox = rcChat.left - rcAbs.left;
    oy = (rcChat.bottom + rcChat.top) / 2 - rcAbs.top;
    cx = 1;
    cy = (rcChat.bottom - rcChat.top) / 2;

    while (true)
    {
        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        // Chat channel background's rgb
        if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33)
        {
            // Continues
        }
        else
        {
            break; // Highlighted message
        }
    }

    // Scrolls down until the bottom of highlighted message
#ifndef _DEBUG
    Tab(hWnd);

    ox = rcChat.left - rcAbs.left;
    oy = rcChat.bottom - rcAbs.top;
    cx = 1;
    cy = 1;

    while (true)
    {
        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        // Chat channel background's rgb
        if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33)
        {
            break;
        }
        else
        {
            PressKey(hWnd, VK_DOWN, FALSE);
        }
    }
#endif

    // Finds selected message highlight's background
    ox = rcChat.left - rcAbs.left;
    oy = rcChat.top - rcAbs.top;
    cx = 1;
    cy = rcChat.bottom - rcChat.top;

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    for (int i = 0; i < cx * cy * 4; i += 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        // Chat channel background's rgb
        if (r == 14 && g == 22 && b == 33)
        {
            if (rcMsg.top)
            {
                rcMsg.bottom = rcAbs.top + y - 1;

                break;
            }
        }
        else if (!rcMsg.top)
        {
            rcMsg.top = rcAbs.top + y;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    // Waits until the scrolling animation stops
#ifndef _DEBUG
    while (true)
    {
        std::cout << "Waits" << std::endl;

        Sleep(150);

        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        if (r == pBytes[2 + (rcMsg.bottom - rcAbs.top - oy + 1) * 4])
        {
            // Animation remains unchanged means completed

            break;
        }
        else
        {
            rcMsg.top = 0;

            for (int i = 0; i < cx * cy * 4; i += 4)
            {
                if (r == pBytes[i + 2]
                    && g == pBytes[i + 1]
                    && b == pBytes[i])
                {
                    continue;
                };

                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;
                a = pBytes[i + 3];
                r = pBytes[i + 2];
                g = pBytes[i + 1];
                b = pBytes[i];

                // Chat channel background's rgb
                if (r == 14 && g == 22 && b == 33)
                {
                    if (rcMsg.top)
                    {
                        rcMsg.bottom = rcAbs.top + y - 1;

                        break;
                    }
                }
                else if (!rcMsg.top)
                {
                    rcMsg.top = rcAbs.top + y;
                }
            }
        }
    }

    // Performs adjustment: Horizontal
    ox = rcChat.left - rcAbs.left + 64; // Near inside left of message 
    oy = rcMsg.bottom - rcAbs.top - 24; // Near bottom of message
    cx = rcChat.right - rcAbs.left - ox;
    cy = 1;

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    r = pBytes[2 + (cx * cy - 1) * 4]; g = pBytes[1 + (cx * cy - 1) * 4]; b = pBytes[(cx * cy - 1) * 4];

    for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;

        rcMsg.right = rcAbs.left + x;
        rcMsg.left = rcChat.left + 58;

        break;
    }

    // Performs adjustment: Vertical
    ox = rcMsg.right - rcAbs.left;
    oy = rcMsg.bottom - rcAbs.top;

    CaptureWindow(hWnd, ox, oy, 1, 1, &pBytes[0]);

    if ((pBytes[2] == 43 && pBytes[1] == 82 && pBytes[0] == 120) // My message background's rgb
        || (pBytes[2] == 24 && pBytes[1] == 37 && pBytes[0] == 51)) // Other message background's rgb
    {
        std::cout << "Going down" << std::endl;

        pBytesTemp[2] = pBytes[2]; pBytesTemp[1] = pBytes[1]; pBytesTemp[0] = pBytes[0]; // Caches message background's rgb

        // Go down
        ox = ox;
        oy = oy;
        cx = 1;
        cy = rcChat.bottom - rcMsg.bottom;

        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        for (int i = 0; i < cx * cy * 4; i += 4)
        {
            if ((pBytes[2 + i] == 43 && pBytes[1 + i] == 82 && pBytes[i] == 120) // My message background's rgb
                || (pBytes[2 + i] == 24 && pBytes[1 + i] == 37 && pBytes[i] == 51)) // Other message background's rgb
            {
                // Continues
            }
            else
            {
                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;

                rcMsg.bottom = rcAbs.top + y;

                break;
            }
        }
    }
    else
    {
        std::cout << "Going up" << std::endl;

        // Go up
        ox = ox;
        oy = rcMsg.top - rcAbs.top;
        cx = 1;
        cy = rcMsg.bottom - rcMsg.top;

        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
        {
            if ((pBytes[2 + i] == 43 && pBytes[1 + i] == 82 && pBytes[i] == 120) // My message background's rgb
                || (pBytes[2 + i] == 24 && pBytes[1 + i] == 37 && pBytes[i] == 51)) // Other message background's rgb
            {
                pBytesTemp[2] = pBytes[2 + i]; pBytesTemp[1] = pBytes[1 + i]; pBytesTemp[0] = pBytes[i]; // Caches message background's rgb

                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;

                rcMsg.bottom = rcAbs.top + y + 1;

                break;
            }
        }
    }

    // DEPRECATED: Checks reply's icon
    if (false)
    {
        ox = rcMsg.right - rcAbs.left - 128; // Approaching reply's icon
        oy = rcMsg.bottom - rcAbs.top - 12; // Near bottom of message
        cx = 28;
        cy = 18;

        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        for (int i = 0; i < cx * cy * 4; i += 4)
        {
            std::cout << (i / 4) % cx + ox << ", " << (i / 4) / cx + oy << ": " << (int)pBytes[2 + i] << ", " << (int)pBytes[1 + i] << ", " << (int)pBytes[i] << std::endl;

            // reply icon's rgb = 106, 190, 254
            if (abs(pBytes[i + 2] - 106) + abs(pBytes[i + 1] - 190) + abs(pBytes[i] - 254) < 10)
            {
                //hasReplies = TRUE;

                break;
            }
        }

        // Near reply's icon
        x = rcMsg.right - rcAbs.left - 24;
        y = rcMsg.bottom - rcAbs.top - 12;
    }

    // Left bottom of message
    x = rcMsg.left - rcAbs.left;
    y = rcMsg.bottom - rcAbs.top;

    // Checks "view replies" option
    RClick(hWnd, x, y);

    if (GetTgContextMenuY(hWnd, 7 + (pBytesTemp[2] == 43)) == 0) // No "View replies" option found
    {
        Click(hWnd, x + 24, GetTgContextMenuY(hWnd, 0)); // Clicks "Reply"

        OpenClipboard(NULL);
        wcscpy_s(clipboard, (wchar_t*)GetClipboardData(CF_UNICODETEXT));
        wcscat_s(clipboard, L"\n催促");
        CloseClipboard();

        CopyToClipboard(clipboard);

        Click(hWnd, (rcChat.right + rcChat.left) / 2 - rcAbs.left, (rcAbs.bottom - rcAbs.top) - 12);
        Click(hWnd, (rcChat.right + rcChat.left) / 2 - rcAbs.left, (rcAbs.bottom - rcAbs.top) - 12);
        Click(hWnd, (rcChat.right + rcChat.left) / 2 - rcAbs.left, (rcAbs.bottom - rcAbs.top) - 12); // Triple clicks to highlight all

        Paste(hWnd); // Overwrites previous draft
        Enter(hWnd); // Sends message

        wcscpy_s(clipboard, L"-3");
        CopyToClipboard(clipboard);

        goto ending;
    }

    Click(hWnd, x + 24, GetTgContextMenuY(hWnd, 1)); // Clicks "View replies"

    // Waits discussion channel loads
    ox = rcMsg.right - rcAbs.left - 24; // Counts as loaded once covers up to 80% of the horizontal view
    oy = (rcMsg.bottom + rcMsg.top) / 2 - rcAbs.top;
    cx = rcChat.right - rcAbs.left - ox - 12; // Omits scrollbar
    cy = 1;

    SetCursorPos(rcAbs.left + ox, rcAbs.top + oy);

    while (!isLoading)
    {
        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        isLoading = TRUE;

        for (int i = (cx * cy - 1) * 4; i > 0; i -= 4)
        {
            // Chat channel background's rgb
            if (pBytes[i + 2] == 14 && pBytes[i + 1] == 22 && pBytes[i] == 33)
            {
                // Continutes
            }
            else
            {
                Sleep(250);

                isLoading = FALSE;

                break;
            }
        }
    }

    // Need to cancel reply-to
    CancelTgReply(hWnd);

    // Waits channel's last message loads
    ox = rcChat.left - rcAbs.left;
    oy = rcChat.bottom - rcAbs.top - 24; // Bottom of last message
    cx = rcChat.right - rcChat.left - 12; // Omits scrollbar
    cy = 1;

    SetCursorPos(rcAbs.left + ox, rcAbs.top + oy);

    while (isLoading)
    {
        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        for (int i = 0; i < cx * cy * 4; i += 4)
        {
            // Chat channel background's rgb
            if (pBytes[i + 2] == 14 && pBytes[i + 1] == 22 && pBytes[i] == 33)
            {
                // Continutes
            }
            else
            {
                isLoading = FALSE;

                break;
            }
        }
    }

    // Waits canvas jumps to bottom
    ox = rcChat.left - rcAbs.left + 52 + 12;
    oy = rcChat.bottom - rcAbs.top;

    SetCursorPos(rcAbs.left + ox, rcAbs.top + oy);

    while (true)
    {
        CaptureWindow(hWnd, ox, oy, 1, 1, &pBytes[0]);

        // Chat channel background's rgb
        if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33)
        {
            break;
        }
        else
        {
            PressKey(hWnd, VK_DOWN, FALSE);
        }
    }

    // Waits messages above to load
    ox = rcChat.left - rcAbs.left + 52 + 12;
    oy = (rcChat.bottom + rcChat.top) / 2 - rcAbs.top;
    cx = 1;
    cy = (rcChat.bottom - rcChat.top) / 2;

    SetCursorPos(rcAbs.left + ox, rcAbs.top + oy);

    isLoading = TRUE;

    while (isLoading)
    {
        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        for (int i = 0; i < cx * cy * 4; i += 4)
        {
            // Chat channel background's rgb
            if (pBytes[i + 2] == 14 && pBytes[i + 1] == 22 && pBytes[i] == 33)
            {
                // Waits
            }
            else if ((pBytes[i + 2] == 43 && pBytes[i + 1] == 82 && pBytes[i] == 120) // My message background's rgb
                || (pBytes[i + 2] == 24 && pBytes[i + 1] == 37 && pBytes[i] == 51)) // Other message background's rgb
            {
                isLoading = FALSE;

                break;
            }
            else
            {
                // Might be pinned message at the top (Excluded the upper section of the chat)
                // Waits

                isLoading = FALSE;

                break;
            }
        }
    }
#endif

    // Finds other last message
    ox = rcChat.left - rcAbs.left + 52 + 8;
    oy = rcChat.top - rcAbs.top;
    cx = 1;
    cy = (rcChat.bottom - rcChat.top);

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    std::cout << r << ", " << g << ", " << b << std::endl;

    r = g = b = -1;
    pBytesTemp[2] = pBytesTemp[1] = pBytesTemp[0] = -1;
    rcMsg = {};

    for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        if (!(r == 30 && g == 44 && b == 58) // Bot button background's rgb
            && !(r == 14 && g == 22 && b == 33) // Chat channel background's rgb
            && !(r == pBytesTemp[2] && g == pBytesTemp[1] && b == pBytesTemp[0])) // Might be highlighted background's rgb
        {
            if (!(r == 43 && g == 82 && b == 120) // My message background's rgb
                && !(r == 24 && g == 37 && b == 51)) // Other message background's rgb
            {
                pBytesTemp[2] = r; pBytesTemp[1] = g; pBytesTemp[0] = b; // Might be highlighted background's rgb

                i += 4;
            }

            if (!rcMsg.bottom)
            {
                rcMsg.bottom = rcAbs.top + y;
            }
        }
        else
        {
            if (rcMsg.bottom)
            {
                rcMsg.top = rcAbs.top + y + 1;

                if (rcMsg.bottom - rcMsg.top < 16)
                {
                    rcMsg.top = NULL;
                    rcMsg.bottom = NULL;
                }
                else if (pBytes[2 + (rcMsg.top - rcAbs.top - oy) * 4] == 43 // My message background's rgb
                    || pBytes[2 + (rcMsg.bottom - rcAbs.top - oy) * 4] == 43)
                {
                    rcMsg.top = NULL;
                    rcMsg.bottom = NULL;
                }
                else
                {
                    break;
                }
            }
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    if (!rcMsg.bottom) {
#ifndef _DEBUG
        OpenClipboard(NULL);
        wcscpy_s(clipboard, L"Me\n");
        wcscat_s(clipboard, (wchar_t*)GetClipboardData(CF_UNICODETEXT));
        CloseClipboard();

        CopyToClipboard(clipboard);

        Sleep(150); // Ensures text is copied
        Click(hWndExt, X, Y + 1); // Tells extension to cache copied text
        Sleep(150); // Ensures text is cached by extension
#endif

        goto ending;
    }
    else if (!rcMsg.top)
    {
        rcMsg.top = rcChat.top;
    }

    // DEBUG: Check detected message position
    SetCursorPos(rcChat.left + 52, rcMsg.top);
    Sleep(100);
    SetCursorPos(rcChat.left + 52, rcMsg.bottom);
    Sleep(100);

    // Performs adjustment: Vertical
    r = pBytes[2 + (rcMsg.bottom - rcAbs.top - oy) * 4];
    g = pBytes[1 + (rcMsg.bottom - rcAbs.top - oy) * 4];
    b = pBytes[0 + (rcMsg.bottom - rcAbs.top - oy) * 4];

    if (r == pBytes[2 + (rcMsg.top - rcAbs.top - oy) * 4]
        && g == pBytes[1 + (rcMsg.top - rcAbs.top - oy) * 4]
        && b == pBytes[0 + (rcMsg.top - rcAbs.top - oy) * 4])
    {
        if (!(r == 43 && g == 82 && b == 120)  // My message background's rgb
            && !(r == 24 && g == 37 && b == 51))  // Other message background's rgb
        {
            std::cout << "Message background highlighted" << std::endl;

            pBytesTemp[2] = r; pBytesTemp[1] = g; pBytesTemp[0] = b; // Caches highlighted background's rgb

            for (int i = (rcMsg.bottom - rcAbs.top - oy) * 4; i > 0; i -= 4) // Adjust bottom
            {
                if (r == pBytes[i + 2]
                    && g == pBytes[i + 1]
                    && b == pBytes[i])
                {
                    continue;
                }

                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;

                std::cout << x << ", " << y << ": " << (int)pBytes[i + 2] << ", " << (int)pBytes[i + 1] << ", " << (int)pBytes[i] << std::endl;

                rcMsg.bottom = rcAbs.top + y;

                break;
            }

            for (int i = (rcMsg.bottom - rcAbs.top - oy) * 4; i > 0; i -= 4) // Adjust top
            {
                if (r == pBytes[i + 2]
                    && g == pBytes[i + 1]
                    && b == pBytes[i])
                {
                    x = (i / 4) % cx + ox;
                    y = (i / 4) / cx + oy;

                    std::cout << x << ", " << y << ": " << (int)pBytes[i + 2] << ", " << (int)pBytes[i + 1] << ", " << (int)pBytes[i] << std::endl;

                    rcMsg.top = rcAbs.top + y + 1;

                    break;
                }
            }
        }
        else
        {
            pBytesTemp[2] = 14; pBytesTemp[1] = 22; pBytesTemp[0] = 33; // Caches channel background's rgb
        }
    }

    // DEBUG: Check vertically adjusted message position
    SetCursorPos(rcChat.left + 52, rcMsg.top);
    Sleep(100);
    SetCursorPos(rcChat.left + 52, rcMsg.bottom);
    Sleep(100);

    // Performs adjustment: Horizontal
    ox = rcChat.left - rcAbs.left + 52 + 6; // Near left of message 
    oy = rcMsg.bottom - rcAbs.top - 24; // Near bottom of message
    cx = rcChat.right - rcAbs.left - ox;
    cy = 1;

    CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

    r = pBytes[2 + (cx * cy - 1) * 4]; g = pBytes[1 + (cx * cy - 1) * 4]; b = pBytes[(cx * cy - 1) * 4];

    for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;

        rcMsg.right = rcAbs.left + x;
        rcMsg.left = rcChat.left + 58;

        break;
    }

    // DEBUG: Check horizontally adjusted message position
    SetCursorPos(rcMsg.left, rcMsg.top);
    Sleep(100);
    SetCursorPos(rcMsg.right, rcMsg.top);
    Sleep(100);
    SetCursorPos(rcMsg.left, rcMsg.bottom);
    Sleep(100);
    SetCursorPos(rcMsg.right, rcMsg.bottom);
    Sleep(100);

    // Gets message bottom-left position
#ifndef _DEBUG
    x = (rcMsg.left - rcAbs.left);
    y = (rcMsg.bottom - rcAbs.top);

    RClick(hWnd, x, y);

    if (pBytes[2] == 43) // My message background's rgb
    {
        Click(hWnd, x + 24, GetTgContextMenuY(hWnd, 2)); // Copies last message' content

        OpenClipboard(NULL);
        wcscpy_s(clipboard, L"Me\n");
        wcscat_s(clipboard, (wchar_t*)GetClipboardData(CF_UNICODETEXT));
        CloseClipboard();

        CopyToClipboard(clipboard);
    }
    else if (pBytes[2] == 24) // Other message background's rgb
    {
        Click(hWnd, x + 24, GetTgContextMenuY(hWnd, 1)); // Copies last message' content
    }
    else // Images
    {
        wcscpy_s(clipboard, L"-4");
        CopyToClipboard(clipboard);

        goto ending;
    }

    Sleep(150); // Ensures text is copied
    Click(hWndExt, X, Y + 1); // Tells extension to cache copied text
    Sleep(150); // Ensures text is cached by extension
#endif

    // Copies attachments
    sides[0][0] = rcMsg.left - rcAbs.left;
    sides[0][1] = rcMsg.top - rcAbs.top - 1;
    sides[0][2] = 1;
    sides[0][3] = rcMsg.bottom - rcMsg.top + 2;

    sides[1][0] = rcChat.left - rcAbs.left + 50;
    sides[1][1] = rcMsg.top - rcAbs.top; // Need to be adjusted after first side
    sides[1][2] = rcMsg.right - rcChat.left - 50;
    sides[1][3] = 1;

    sides[2][0] = rcMsg.right - rcAbs.left - 4;
    sides[2][1] = rcMsg.top - rcAbs.top - 2; // Need to be adjusted after first side
    sides[2][2] = 1;
    sides[2][3] = rcMsg.bottom - rcMsg.top + 2; // Need to adjust along with oy

    sides[3][0] = rcChat.left - rcAbs.left + 46; // Includes the tip of message pointer
    sides[3][1] = rcMsg.bottom - rcAbs.top; // Need to be adjust to the bottom of image
    sides[3][2] = rcMsg.right - rcChat.left - 46;
    sides[3][3] = 1;

    std::cout << (int)pBytesTemp[2] << ", " << (int)pBytesTemp[1] << ", " << (int)pBytesTemp[0] << std::endl;

    for (int *side : sides)
    {
        ox = side[0];
        oy = side[1];
        cx = side[2];
        cy = side[3];

        SetForegroundWindow(hWnd);
        CaptureWindow(hWnd, ox, oy, cx, cy, &pBytes[0]);

        r = pBytesTemp[2]; g = pBytesTemp[1]; b = pBytesTemp[0];

        std::cout << std::endl;
        std::cout << ox << ", " << oy << ", " << cx << ", " << cy << std::endl;

        for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
        {
            if (r == pBytes[i + 2]
                && g == pBytes[i + 1]
                && b == pBytes[i])
            {
                continue;
            }

            std::cout << (i / 4) % cx + ox << ", " << (i / 4) / cx + oy << ": " << (int)pBytes[2 + i] << ", " << (int)pBytes[1 + i] << ", " << (int)pBytes[i] << std::endl;

            x = (i / 4) % cx + ox;
            y = (i / 4) / cx + oy;

            if ((r == 43 && g == 82 && b == 120) // My message background's rgb
                || (r == 24 && g == 37 && b == 51)) // Other message background's rgb
            {
                if (pBytes[i + 2] == pBytesTemp[2] && pBytes[i + 1] == pBytesTemp[1] && pBytes[i] == pBytesTemp[0]) // Chat channel background's rgb
                {
                    // Message ends
                    std::cout << "Ends with only text" << std::endl;

                    if (&side[0] == &sides[0][0])
                    {
                        std::cout << "Adjust top of message" << std::endl;

                        rcMsg.top = rcAbs.top + y + 1;

                        // Updates remaining sides
                        sides[1][1] = max(rcMsg.top - rcAbs.top, sides[1][1]);
                        sides[2][1] = rcMsg.top - rcAbs.top - 2;
                        sides[2][3] = rcMsg.bottom - rcMsg.top + 2;
                    }
                    
                    break;
                }
                else if ((min(pBytesTemp[2], r) <= pBytes[i + 2] && pBytes[i + 2] <= max(pBytesTemp[2], r))
                    && (min(pBytesTemp[1], g) <= pBytes[i + 1] && pBytes[i + 1] <= max(pBytesTemp[1], g))
                    && (min(pBytesTemp[0], b) <= pBytes[i] && pBytes[i] <= max(pBytesTemp[0], b))) // Fading between channel and message background's rgb
                {
                    continue;
                }
                else if (y - oy < 64)
                {
                    continue;
                }
                else // Images with height > 64px
                {
                    std::cout << "Start of image" << std::endl;

                    if (&side[0] == &sides[0][0])
                    {
                        std::cout << "Update bottom of images" << std::endl;

                        // Updates remaining sides
                        sides[3][1] = y;

                        std::cout << "Copy image" << std::endl;

#ifndef _DEBUG
                        RClick(hWnd, x + 1, y);

                        if (GetTgContextMenuY(hWnd, 6 + (r == 43)) != 0) // At least 7 rows (+1 for my message) indicating a image attachment
                        {
                            if (r == 43) // My message background's rgb
                            {
                                Click(hWnd, x + 1 + 24, GetTgContextMenuY(hWnd, 3)); // Copies images
                            }
                            else
                            {
                                Click(hWnd, x + 1 + 24, GetTgContextMenuY(hWnd, 2));
                            }
                        }
                        else
                        {
                            wcscpy_s(clipboard, L"-2");
                            CopyToClipboard(clipboard);
                        }

                        Click(hWndExt, X, Y + 1); // Tells extension to cache copied image
#endif

                    }
                }
            }
            else if (r == pBytesTemp[2] && g == pBytesTemp[1] && b == pBytesTemp[0]) // Chat channel background's rgb
            {
                // Still below bottom of message

                if ((pBytes[i + 2] == 43 && pBytes[i + 1] == 82 && pBytes[i] == 120) // My message background's rgb
                    || (pBytes[i + 2] == 24 && pBytes[i + 1] == 37 && pBytes[i] == 51)) // Other message background's rgb
                {
                    std::cout << "Start of message" << std::endl;
                }
                else if (abs(pBytes[i + 2] - pBytesTemp[2]) + abs(pBytes[i + 1] - pBytesTemp[1]) + abs(pBytes[i] - pBytesTemp[0]) < 10
                    || abs(pBytes[i + 2] - 43) + abs(pBytes[i + 1] - 82) + abs(pBytes[i] - 120) < 10
                    || abs(pBytes[i + 2] - 24) + abs(pBytes[i + 1] - 37) + abs(pBytes[i] - 51) < 10) // Margin of error
                {
                    continue;
                }
                else if ((min(pBytesTemp[2], 43) <= pBytes[i + 2] && pBytes[i + 2] <= max(pBytesTemp[2], 43))
                    && (min(pBytesTemp[1], 82) <= pBytes[i + 1] && pBytes[i + 1] <= max(pBytesTemp[1], 82))
                    && (min(pBytesTemp[0], 120) <= pBytes[i] && pBytes[i] <= max(pBytesTemp[0], 120))) // Fading to my message background's rgb
                {
                    continue;
                }
                else if ((min(pBytesTemp[2], 24) <= pBytes[i + 2] && pBytes[i + 2] <= max(pBytesTemp[2], 24))
                    && (min(pBytesTemp[1], 37) <= pBytes[i + 1] && pBytes[i + 1] <= max(pBytesTemp[1], 37))
                    && (min(pBytesTemp[0], 51) <= pBytes[i] && pBytes[i] <= max(pBytesTemp[0], 51))) // Fading to other message background's rgb
                {
                    continue;
                }
                else // Images
                {
                    std::cout << "Start of image" << std::endl;
                }
            }
            else // Images
            {
                if (pBytes[i + 2] == pBytesTemp[2] && pBytes[i + 1] == pBytesTemp[1] && pBytes[i] == pBytesTemp[0]) // Chat channel background's rgb
                {
                    // Message ends
                    std::cout << "Ends with images" << std::endl;

                    if (&side[0] == &sides[0][0])
                    {
                        std::cout << "Adjust top of message" << std::endl;

                        rcMsg.top = rcAbs.top + y + 1;

                        // Updates remaining sides
                        sides[1][1] = max(rcMsg.top - rcAbs.top, sides[1][1]);
                        sides[2][1] = rcMsg.top - rcAbs.top - 2;
                        sides[2][3] = rcMsg.bottom - rcMsg.top + 2;
                    }

                    break;
                }
                else if ((pBytes[i + 2] == 43 && pBytes[i + 1] == 82 && pBytes[i] == 120) // My message background's rgb
                    || (pBytes[i + 2] == 24 && pBytes[i + 1] == 37 && pBytes[i] == 51)) // Other message background's rgb
                {
                    if ((pBytes[(i - 20) + 2] == 43 && pBytes[(i - 20) + 1] == 82 && pBytes[(i - 20)] == 120)
                        || (pBytes[(i - 20) + 2] == 24 && pBytes[(i - 20) + 1] == 37 && pBytes[(i - 20)] == 51))
                    {
                        // Message's top
                        std::cout << "Approaches top of message" << std::endl;

                        if (&side[0] == &sides[0][0])
                        {
                            std::cout << "Adjust top of message to top of image" << std::endl;

                            sides[1][1] = y + 1;
                        }
                    }
                    else
                    {
                        // Images divider
                        std::cout << "Image divider" << std::endl;

                        if (&side[0] != &sides[3][0])
                        {
                            std::cout << "Copy image" << std::endl;

#ifndef _DEBUG
                            RClick(hWnd, x + 1, y + 1);

                            if (GetTgContextMenuY(hWnd, 6 + (r == 43)) != 0) // At least 7 rows (+1 for my message) indicating a image attachment
                            {
                                if (r == 43) // My message background's rgb
                                {
                                    Click(hWnd, x + 1 + 24, GetTgContextMenuY(hWnd, 3)); // Copies images
                                }
                                else
                                {
                                    Click(hWnd, x + 1 + 24, GetTgContextMenuY(hWnd, 2));
                                }
                            }
                            else
                            {
                                wcscpy_s(clipboard, L"-2");
                                CopyToClipboard(clipboard);
                            }

                            Click(hWndExt, X, Y + 1); // Tells extension to cache copied image
#endif
                        }
                    }
                }
            }

            a = pBytes[i + 3];
            r = pBytes[i + 2];
            g = pBytes[i + 1];
            b = pBytes[i];
        }
    }

ending:
#ifndef _DEBUG

    SetForegroundWindow(hWnd);
    Click(hWndExt, X, Y + 1); // Tells extension to verify

    ULONGLONG lastTime = GetTickCount64();

    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    while (true)
    {
        CaptureWindow(hWndExt, X, Y, 1, 1, &pBytes[0]);

        r = pBytes[2];
        g = pBytes[1];
        b = pBytes[0];

        if (r == 0 && g == 70 && b == 255)
        {
            // Waits
            if (lastTime && GetTickCount64() - lastTime > 500)
            {
                lastTime = 0;

                Click(hWndExt, X, Y + 1); // Tells extension to verify once again
            }
        }
        else if (r == 0 && g == 255 && b == 255) // Extension replying
        {
            // Waits extension to reply
        }
        else
        {
            break;
        }
    }

    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    if (r == 0 && g == 140 && b == 255) // Sends copied message to Telegram
    {
        Click(hWnd, (rcChat.right + rcChat.left) / 2 - rcAbs.left, (rcAbs.bottom - rcAbs.top) - 12);
        Click(hWnd, (rcChat.right + rcChat.left) / 2 - rcAbs.left, (rcAbs.bottom - rcAbs.top) - 12); // Double clicks to highlight

        Paste(hWnd); // Paste clipboard set by extension

        Enter(hWnd);

        Click(hWndExt, X, Y + 1); // Informs extension regarding completion
    }
    else if (r == 100 && g == 0 && b == 255) // Replied by extension
    {
        // Does nothing
    }
    else // Ignores
    {

    }
#endif

#ifdef _DEBUG
    CaptureWindow(hWnd, rcMsg.left - rcAbs.left, rcMsg.top - rcAbs.top, rcMsg.right - rcMsg.left, rcMsg.bottom - rcMsg.top, &pBytes[0]);

    return CWE_TERMINATE;
#endif

    return result;
}

//
//  FUNCTION: ToggleButton(HWND)
//
//  PURPOSE: Toggles the button on/off
//
int ToggleButton(HWND hWnd)
{
    isCapturing = !isCapturing;

    SetForegroundWindow(hWnd);

    SendMessage(hBtnAuto, BM_SETSTATE, (WPARAM)isCapturing, NULL);

    return 0;
}

//
//  FUNCTION: CopyToClipboard(wchar_t*)
//
//  PURPOSE: Copy content to clipboard
//
int CopyToClipboard(wchar_t* text)
{
    const size_t len = sizeof(WCHAR) * (wcslen(text) + 1);
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

    memcpy(GlobalLock(hMem), text, len);
    GlobalUnlock(hMem);

    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();

    return 0;
}

//
//  FUNCTION: SendInputByBlocking(UINT, LPINPUT, int, BOOL)
//
//  PURPOSE: Temporary blocks user input to trigger sequences of events
//
int SendInputByBlocking(_In_ UINT cInputs, _In_reads_(cInputs) LPINPUT pInputs, _In_ int cbSize, BOOL cd)
{
    UINT uSent;

    BlockInput(TRUE);

    uSent = SendInput(cInputs, pInputs, cbSize);

    BlockInput(FALSE);

    if (cd)
        Sleep(150);

    return uSent;
}

//
//  FUNCTION: SendInputByBlocking(UINT, LPINPUT, int)
//
//  PURPOSE: Temporary blocks user input to trigger sequences of events
//
int SendInputByBlocking(_In_ UINT cInputs, _In_reads_(cInputs) LPINPUT pInputs, _In_ int cbSize)
{
    return SendInputByBlocking(cInputs, pInputs, cbSize, TRUE);
}

//
//  FUNCTION: Click(int, int)
//
//  PURPOSE: Click on specified screen location
//
int Click(int x, int y)
{
    //SetCursorPos(x, y);

    double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
    double fx = x * (65535.0f / fScreenWidth);
    double fy = y * (65535.0f / fScreenHeight);

    INPUT inputs[3] = {};
    ZeroMemory(inputs, sizeof(inputs));

    // Ensures left mouse is up before moving cursor
    //inputs[0].type = INPUT_MOUSE;
    //inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = fx; // desired X coordinate
    inputs[0].mi.dy = fy; // desired Y coordinate
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        MessageBox(NULL, L"Mouse left click failed", L"Input Error", MB_OK);
    }

    return 0;
}

//  FUNCTION: Click(HWND, int, int)
//
//  PURPOSE: Click on specified window's relative location
//
int Click(HWND hWnd, int x, int y)
{
    RECT rcAbs;
    GetAbsoluteRect(hWnd, &rcAbs);

    if (GetForegroundWindow() != hWnd)
    {
        SetForegroundWindow(hWnd);
        Sleep(100);
    }

    int result = Click(rcAbs.left + x, rcAbs.top + y);

    return result;
}

//
//  FUNCTION: RClick(int, int)
//
//  PURPOSE: Right click on specified screen location
//
int RClick(int x, int y)
{
    //SetCursorPos(x, y);

    double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
    double fx = x * (65535.0f / fScreenWidth);
    double fy = y * (65535.0f / fScreenHeight);

    INPUT inputs[3] = {};
    ZeroMemory(inputs, sizeof(inputs));

    // Ensures left mouse is up before moving cursor
    //inputs[0].type = INPUT_MOUSE;
    //inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = fx; // desired X coordinate
    inputs[0].mi.dy = fy; // desired Y coordinate
    inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        MessageBox(NULL, L"Mouse right click failed", L"Input Error", MB_OK);
    }

    return 0;
}

//  FUNCTION: RClick(HWND, int, int)
//
//  PURPOSE: Right click on specified window's relative location
//
int RClick(HWND hWnd, int x, int y)
{
    RECT rcAbs;
    GetAbsoluteRect(hWnd, &rcAbs);

    if (GetForegroundWindow() != hWnd)
    {
        SetForegroundWindow(hWnd);
        Sleep(100);
    }

    int result = RClick(rcAbs.left + x, rcAbs.top + y);

    return result;
}

//
//  FUNCTION: Drag(int, int, int, int)
//
//  PURPOSE: Trigger drag and drop programmatically
//
int Drag(int x1, int y1, int x2, int y2)
{
    double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
    double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
    double fx1 = x1 * (65535.0f / fScreenWidth);
    double fy1 = y1 * (65535.0f / fScreenHeight);
    double fx2 = x2 * (65535.0f / fScreenWidth);
    double fy2 = y2 * (65535.0f / fScreenHeight);

    INPUT inputs[1] = {};
    INPUT input[6] = {};
    ZeroMemory(input, sizeof(input));

    input[0].type = INPUT_MOUSE;
    input[0].mi.dx = fx1; // desired X1 coordinate
    input[0].mi.dy = fy1; // desired Y1 coordinate
    input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    input[2].type = INPUT_MOUSE;
    input[2].mi.dx = 5;
    input[2].mi.dy = 5;
    input[2].mi.dwFlags = MOUSEEVENTF_MOVE;

    input[3].type = INPUT_MOUSE;
    input[3].mi.dx = 5;
    input[3].mi.dy = 5;
    input[3].mi.dwFlags = MOUSEEVENTF_MOVE;

    input[4].type = INPUT_MOUSE;
    input[4].mi.dx = fx2; // desired X2 coordinate
    input[4].mi.dy = fy2; // desired Y2 coordinate
    input[4].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    input[5].type = INPUT_MOUSE;
    input[5].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    for (int i = 0; i < 6; i++)
    {
        ZeroMemory(inputs, sizeof(inputs));

        inputs[0] = input[i];

        UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        if (uSent != ARRAYSIZE(inputs))
        {
            MessageBox(NULL, L"Mouse drag failed", L"Input Error", MB_OK);

            return 0;
        }
    }

    return 1;
}

//
//  FUNCTION: PressKey(HWND, WORD, BOOL cd)
//
//  PURPOSE: Trigger key press programmatically
//
int PressKey(HWND hWnd, WORD vk, BOOL cd)
{
    if (hWnd && GetForegroundWindow() != hWnd)
    {
        SetForegroundWindow(hWnd);
        Sleep(100);
    }

    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vk;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vk;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT), cd);
    if (uSent != ARRAYSIZE(inputs))
    {
        MessageBox(NULL, L"Key press failed", L"Input Error", MB_OK);
    }

    return uSent == ARRAYSIZE(inputs);
}

//
//  FUNCTION: PressKey(HWND, WORD)
//
//  PURPOSE: Trigger key press programmatically
//
int PressKey(HWND hWnd, WORD vk)
{
    return PressKey(hWnd, vk, TRUE);
}

//
//  FUNCTION: HoldKey(HWND, WORD)
//
//  PURPOSE: Trigger key down programmatically
//
int HoldKey(HWND hWnd, WORD vk)
{
    if (hWnd && GetForegroundWindow() != hWnd)
    {
        SetForegroundWindow(hWnd);
        Sleep(100);
    }

    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vk;

    UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        MessageBox(NULL, L"Key down failed", L"Input Error", MB_OK);
    }

    return uSent == ARRAYSIZE(inputs);
}

//
//  FUNCTION: ReleaseKey(HWND, WORD)
//
//  PURPOSE: Trigger key up programmatically
//
int ReleaseKey(HWND hWnd, WORD vk)
{
    if (hWnd && GetForegroundWindow() != hWnd)
    {
        SetForegroundWindow(hWnd);
        Sleep(100);
    }

    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vk;
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        MessageBox(NULL, L"Key up failed", L"Input Error", MB_OK);
    }

    return uSent == ARRAYSIZE(inputs);
}

//
//  FUNCTION: Paste(HWND)
//
//  PURPOSE: Trigger Ctrl+V programmatically
//
int Paste(HWND hWnd)
{
    if (hWnd && GetForegroundWindow() != hWnd)
    {
        SetForegroundWindow(hWnd);
        Sleep(100);
    }

    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LCONTROL;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LCONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInputByBlocking(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        MessageBox(NULL, L"Paste failed", L"Input Error", MB_OK);
    }

    return uSent == ARRAYSIZE(inputs);
}

//
//  FUNCTION: Enter(HWND)
//
//  PURPOSE: Trigger enter programmatically
//
int Enter(HWND hWnd)
{
    int success = PressKey(hWnd, VK_RETURN);
    
    if (!success)
    {
        MessageBox(NULL, L"Enter failed", L"Input Error", MB_OK);
    }

    return success;
}

//
//  FUNCTION: Tab(HWND)
//
//  PURPOSE: Trigger tab programmatically
//
int Tab(HWND hWnd)
{
    int success = PressKey(hWnd, VK_TAB);

    if (!success)
    {
        MessageBox(NULL, L"Tab failed", L"Input Error", MB_OK);
    }

    return success;
}

//
//  FUNCTION: Esc(HWND)
//
//  PURPOSE: Trigger Esc programmatically
//
int Esc(HWND hWnd)
{
    int success = PressKey(hWnd, VK_ESCAPE);

    if (!success)
    {
        MessageBox(NULL, L"Esc failed", L"Input Error", MB_OK);
    }

    return success;
}

//
//  FUNCTION: ArrowKeyDown(HWND)
//
//  PURPOSE: Trigger down arrow key programmatically
//
int ArrowKeyDown(HWND hWnd)
{
    int success = PressKey(hWnd, VK_DOWN);

    if (!success)
    {
        MessageBox(NULL, L"ArrowKeyDown failed", L"Input Error", MB_OK);
    }

    return success;
}

//
//  FUNCTION: Backspace(HWND hWnd)
//
//  PURPOSE: Trigger backspace programmatically
//
int Backspace(HWND hWnd)
{
    int success = PressKey(hWnd, VK_BACK);

    if (!success)
    {
        MessageBox(NULL, L"Backspace failed", L"Input Error", MB_OK);
    }

    return 0;
}

//
//  FUNCTION: SearchTgGroup(HWND)
//
//  PURPOSE: Search group in telegram Desktop App programmatically
//
int SearchTgGroup(HWND hWndTg)
{
    RECT rcTg;
    int cx;
    int cy;
    int ox;
    int oy;

    GetAbsoluteRect(hWndTg, &rcTg);

    BYTE* pBytes = new BYTE[4];

    // Finds chat bar position
#ifndef _DEBUG
    ox = (rcTg.right - rcTg.left) / 2;
    oy = (rcTg.bottom - rcTg.top) - 5;
    cx = 1;
    cy = 1;

    while (true)
    {
        CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

        // Bottom chat bar's position
        if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33) // Chat background's rgb
            break;
        else
            Esc(hWndTg); // Exits discussion and group channels
    }
#endif

    Click(hWndTg, 200, 52); // Triple clicks to highlight previous content
    Click(hWndTg, 200, 52);
    Click(hWndTg, 200, 52);
    Backspace(hWndTg); // Clears all content
    Paste(hWndTg); // Paste to overwrite previous content

    Sleep(500); // Waits Telegram to enter loading status

    while (true)
    {
        CaptureWindow(hWndTg, 200, 84, 1, 1, &pBytes[0]);

        // Loading bar's rgb
        if (pBytes[2] == 32 && pBytes[1] == 43 && pBytes[0] == 56)
            ; // Waits
        else
        {
            break;
        }
    }

    Enter(hWndTg);

    return 0;
}

//
//  FUNCTION: SearchTgMessage(HWND)
//
//  PURPOSE: Search message in telegram Desktop App programmatically
//
int SearchTgMessage(HWND hWndTg)
{
    RECT rcTg;
    BOOL searchLoaded = FALSE;
    int cx;
    int cy;
    int ox;
    int oy;
    // Pixels
    int x = 0;
    int y = 0;
    int a = -1;
    int r = -1;
    int g = -1;
    int b = -1;

    GetAbsoluteRect(hWndTg, &rcTg);
    cx = rcTg.right - rcTg.left;
    cy = rcTg.bottom - rcTg.top;

    BYTE* pBytes = new BYTE[4 * cx * cy];

    // Finds chat bar position
#ifndef _DEBUG
    ox = (rcTg.right - rcTg.left) / 2;
    oy = (rcTg.bottom - rcTg.top) - 5;
    cx = 1;
    cy = 1;

    while (true)
    {
        CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

        // Bottom chat bar's position
        if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33) // Chat background's rgb
            break;
        else
            Esc(hWndTg); // Exits discussion and group channels
    }
#endif

    // DEBUG: Replace orignal position with sample
    cx = (rcTg.right - rcTg.left) / 2;
    cy = 1;
    ox = 300;
    oy = 64 + 34 - 4;

    // Finds end of search bar
#ifndef _DEBUG
    cx = (rcTg.right - rcTg.left) / 2;
    cy = 1;
    ox = 300;
    oy = 34 - 4;
#endif

    CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

    for (int i = 0; i < cx * cy * 4; i += 4)
    {
        // Search bar's background rgb
        if (pBytes[i + 2] == 23 && pBytes[i + 1] == 33 && pBytes[i] == 43)
        {
            x = (i / 4) % cx + ox;
            y = (i / 4) / cx + oy;

            continue; // Ignores
        }
        else
        {
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

#ifndef _DEBUG
    Esc(hWndTg); // Exits discussion and group channels
    Esc(hWndTg);
    //Click(hWndTg, x - 24, 52); // Click on 'X' to clear search bar
    Click(hWndTg, 200, 52); // Triple clicks to highlight previous content
    Click(hWndTg, 200, 52);
    Click(hWndTg, 200, 52);
    Backspace(hWndTg); // Clears all content
    Paste(hWndTg); // Paste to overwrite previous content
#endif

    // Finds lowest loading bar
    while (!searchLoaded)
    {
        cx = 1;
        cy = rcTg.bottom - rcTg.top;
        ox = x;
        oy = 0;

        CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

        // Gets lowest loading bar
        for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
        {
            // Loading bar background's rgb
            if (pBytes[i + 2] == 32 && pBytes[i + 1] == 43 && pBytes[i] == 56)
            {
                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;

                cx = 12;
                cy = 1;
                ox = x - 24;
                oy = y - 12;

                CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

                // Check if "All chats" text appears in loading bar
                for (int i = 0; i < cx * cy * 4; i += 4)
                {
                    // Loading bar background's rgb
                    if (pBytes[i + 2] == 32 && pBytes[i + 1] == 43 && pBytes[i] == 56)
                        ; // Waits
                    else
                    {
                        searchLoaded = TRUE;
                        break;
                    }

                    std::cout << (i / 4) % cx + ox << ", " << (i / 4) / cx + oy << ": " << (int)pBytes[i + 2] << ", " << (int)pBytes[i + 1] << ", " << (int)pBytes[i] << std::endl;
                }

                break;
            }
        }
    }

    Click(hWndTg, x - 4, rcTg.bottom - rcTg.top - 4);

    // DEPRECATED: Checks for result's scrollbar
    if (false) {
        ox = x - 4;

        CaptureWindow(hWndTg, ox, oy, 1, 1, &pBytes[0]);

        // Loading bar background's rgb
        if (pBytes[2] == 32 && pBytes[1] == 43 && pBytes[0] == 56)
            MessageBox(NULL, L"No scroll bar", L"", MB_OK)
            ; // No scrollbar
        else
        {
            // Finds scrollbar
            //return 1;

            MessageBox(NULL, L"Found scroll bar", L"", MB_OK);

            Click(hWndTg, ox + 24, rcTg.bottom - rcTg.top - 24);

            Sleep(5000);

            while (false)
            {
                Sleep(50);

                CaptureWindow(hWndTg, ox, oy, 1, 1, &pBytes[0]);

                if (pBytes[2] == 32 && pBytes[1] == 43 && pBytes[0] == 56)
                    break;
                else
                    PressKey(hWndTg, VK_DOWN, FALSE);
            }
        }
    }

    // DEPRECATED: Finds blue double ticks
    cx = 1;
    cy = rcTg.bottom - rcTg.top - y;
    ox = x - 68;
    oy = y;

    // DEBUG: Replace orignal position with sample
    cx = 1;
    cy = rcTg.bottom - rcTg.top - y - 72;
    ox = x - 14;
    oy = y;

#ifndef _DEBUG
    // Finds any most bottom texts
    cx = 1;
    cy = rcTg.bottom - rcTg.top - y - 4;
    ox = x - 14;
    oy = y;
#endif

    CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

    for (int i = (cx * cy * 4) - 4; i > -1; i -= 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        // DEPRECATED: Double tick's rgb = 114, 188, 253
        if (false && r == 114 && g == 188 && b == 253)
        {
            break;
        }
        
        if (!(r == 23 && g == 33 && b == 43)) // Search list background's rgb
        {
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    if (y == oy) // No record found
    {
        return 2;
    }

#ifndef _DEBUG
    Click(hWndTg, x, y);
#endif

    return 0;
}

//
//  FUNCTION: PasteTgMessage(HWND)
//
//  PURPOSE: Send message in telegram Desktop App programmatically
//
int PasteTgMessage(HWND hWndTg)
{
    BYTE* pBytes = new BYTE[4];
    RECT rcTg;
    int cx;
    int cy;

    GetAbsoluteRect(hWndTg, &rcTg);
    cx = rcTg.right - rcTg.left;
    cy = rcTg.bottom - rcTg.top;

    Click(hWndTg, cx / 1.5, cy - 18);
    Click(hWndTg, cx / 1.5, cy - 18);
    Click(hWndTg, cx / 1.5, cy - 18); // Triple clicks to highlight all

    // Makes channel canvas to bottom
    Backspace(hWndTg);
    Enter(hWndTg);

    // Checks for reply-to
    CancelTgReply(hWndTg);

    Paste(hWndTg);

    return 0;
}

//
//  FUNCTION: CancelTgReply(HWND)
//
//  PURPOSE: Cancels reply-to in telegram Desktop App programmatically
//
int CancelTgReply(HWND hWndTg)
{
    BYTE* pBytes = new BYTE[4];
    RECT rcTg;
    int cx;
    int cy;

    GetAbsoluteRect(hWndTg, &rcTg);
    cx = rcTg.right - rcTg.left;
    cy = rcTg.bottom - rcTg.top;

    // Checks for reply-to
    CaptureWindow(hWndTg, cx - 12, cy - 48, 1, 1, &pBytes[0]);

    // Chat channel background's rgb
    if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33)
        ;
    else
    {
        Click(hWndTg, cx - 24, rcTg.bottom - 86); // Cancels reply-to
    }

    return 0;
}

//
//  FUNCTION: GetTgContextMenuY(HWND, int)
//
//  PURPOSE: Gets the top y-position of Telegram context menu
//
int GetTgContextMenuY(HWND hWndTg, int row)
{
    HWND hWndTgPopup = FindWindow(TELEGRAM_POPUP_CLASSNAME, TELEGRAM_POPUP_WINDOWNAME);
    RECT rcPopup;
    RECT rcTg;
    RECT rcRows = {};

    int x = 0;
    int y = 0;
    int ox;
    int oy;
    int cx;
    int cy;
    int r = -1;
    int g = -1;
    int b = -1;

    BOOL isLoaded = FALSE;

    GetAbsoluteRect(hWndTgPopup, &rcPopup);
    GetAbsoluteRect(hWndTg, &rcTg);

    // DEBUG:
    ox = 32 + 240; // Pop up left boundary
    oy = 96 + 220; // First option of pop up
    cx = 1;
    cy = rcPopup.bottom - rcPopup.top - oy;

#ifndef _DEBUG
    ox = 32; // Pop up left boundary
    oy = 96; // First option of pop up
    cx = 1;
    cy = rcPopup.bottom - rcPopup.top - oy;
#endif

    // Finds the position of last option
    BYTE* pBytes = new BYTE[4 * cx * cy];

    while (!isLoaded)
    {
        Sleep(100);

        CaptureWindow(hWndTgPopup, ox, oy, cx, cy, &pBytes[0]);

        for (int i = 0; i < (cx * cy * 4); i += 4)
        {
            if (r == pBytes[i + 2]
                && g == pBytes[i + 1]
                && b == pBytes[i])
            {
                continue;
            }

            if ((i / 4) / cx < 34
                || (oy + (i / 4) / cx) - y < 34)
            {
                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;
                r = pBytes[i + 2];
                g = pBytes[i + 1];
                b = pBytes[i];
            }
            else
            {
                if (rcRows.top == rcPopup.top + y
                    && rcRows.bottom == rcPopup.top + oy + (i / 4) / cx)
                {
                    isLoaded = TRUE;
                }

                rcRows.top = rcPopup.top + y;
                rcRows.bottom = rcPopup.top + oy + (i / 4) / cx;

                break;
            }

            std::cout << x << ", " << y << ": " << (int)pBytes[i + 2] << ", " << (int)pBytes[i + 1] << ", " << (int)pBytes[i] << std::endl;
        }
    }

    if (row == -1)
        return rcRows.bottom - rcTg.top;

    y = (rcRows.top - rcTg.top) + 17 + row * 34;
    
    if (y > rcRows.bottom - rcTg.top)
        return 0;

    return y;
}

//
//  FUNCTION: SearchLarkGroup(HWND)
//
//  PURPOSE: Search group in Lark Desktop App programmatically
//
int SearchLarkGroup(HWND hWndLark)
{
    RECT rcLark;
    int x;
    int y;
    int cx;
    int cy;
    int ox;
    int oy;
    int r = -1;
    int g = -1;
    int b = -1;

    int tabCount = 0;

    GetAbsoluteRect(hWndLark, &rcLark);
    cx = rcLark.right - rcLark.left;
    cy = rcLark.bottom - rcLark.top;

    BYTE* pBytes = new BYTE[4 * cx * cy];
    
#ifndef _DEBUG
    // Triggers general search window
    HoldKey(hWndLark, VK_LCONTROL);
    PressKey(hWndLark, 'K');
    ReleaseKey(hWndLark, VK_LCONTROL);

    // Clears filters
    Click(hWndLark, cx / 2, 86); // Triple clicks to highlight whole content
    Click(hWndLark, cx / 2, 86);
    Click(hWndLark, cx / 2, 86);
    Backspace(hWndLark); // Clears content
    Backspace(hWndLark); // Clears filter

    // Finds filters position
    ox = 100 + 12;
    oy = 50 + 72;
    cx = rcLark.right - rcLark.left - ox;
    cy = 1;

    CaptureWindow(hWndLark, ox, oy, cx, cy, &pBytes[0]);

    r = pBytes[2]; g = pBytes[1]; b = pBytes[0];
    
    for (int i = 0; i < cx * cy * 4; i += 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            i += 24 * 4;

            tabCount++;

            if (tabCount == 5) // Group filter
            {
                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;

                Click(hWndLark, x, y);

                //SetCursorPos(rcLark.left + x, rcLark.top + y);

                break;
            }
        }
    }

    // Pastes group name
    Click(hWndLark, cx / 2, 86);
    Paste(hWndLark);
#endif

#ifndef _DEBUG
    // Finds search result
    ox = 128;
    oy = 186;
    cx = 1;
    cy = 1;

    while (true)
    {
        Sleep(100);

        CaptureWindow(hWndLark, ox, oy, cx, cy, &pBytes[0]);

        if (pBytes[2] == 35 && pBytes[1] == 35 && pBytes[0] == 35) // First group result background's rgb
            break;

        // Checks for Not Found pixels
        ox = (rcLark.right - rcLark.left) / 2 - 52;
        oy = 186;
        cx = 1;
        cy = (rcLark.bottom - rcLark.top) / 2;

        CaptureWindow(hWndLark, ox, oy, cx, cy, &pBytes[0]);

        for (int i = 0; i < cx * cy * 4; i += 4)
        {
            if (pBytes[i + 2] == 65 && pBytes[i + 1] == 204 && pBytes[i] == 185) // Not found's rgb
            {
                return 1;
            }
        }
    }

    Click(hWndLark, ox, oy); // Clicks to navigate
#endif

    return 0;
}

//
//  FUNCTION: SearchLarkMessage(HWND)
//
//  PURPOSE: Search message in Lark Desktop App programmatically
//
int SearchLarkMessage(HWND hWndTg)
{
    RECT rcTg;
    BOOL searchLoaded = FALSE;
    int cx;
    int cy;
    int ox;
    int oy;
    // Pixels
    int x = 0;
    int y = 0;
    int a = -1;
    int r = -1;
    int g = -1;
    int b = -1;

    GetAbsoluteRect(hWndTg, &rcTg);
    cx = rcTg.right - rcTg.left;
    cy = rcTg.bottom - rcTg.top;

    BYTE* pBytes = new BYTE[4 * cx * cy];

    // Finds chat bar position
#ifndef _DEBUG
    ox = (rcTg.right - rcTg.left) / 2;
    oy = (rcTg.bottom - rcTg.top) - 5;
    cx = 1;
    cy = 1;

    while (true)
    {
        CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

        // Bottom chat bar's position
        if (pBytes[2] == 14 && pBytes[1] == 22 && pBytes[0] == 33) // Chat background's rgb
            break;
        else
            Esc(hWndTg); // Exits discussion and group channels
    }
#endif

    // DEBUG: Replace orignal position with sample
    cx = (rcTg.right - rcTg.left) / 2;
    cy = 1;
    ox = 300;
    oy = 64 + 34 - 4;

    // Finds end of search bar
#ifndef _DEBUG
    cx = (rcTg.right - rcTg.left) / 2;
    cy = 1;
    ox = 300;
    oy = 34 - 4;
#endif

    CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

    for (int i = 0; i < cx * cy * 4; i += 4)
    {
        // Search bar's background rgb
        if (pBytes[i + 2] == 23 && pBytes[i + 1] == 33 && pBytes[i] == 43)
        {
            x = (i / 4) % cx + ox;
            y = (i / 4) / cx + oy;

            continue; // Ignores
        }
        else
        {
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

#ifndef _DEBUG
    Esc(hWndTg); // Exits discussion and group channels
    Esc(hWndTg);
    //Click(hWndTg, x - 24, 52); // Click on 'X' to clear search bar
    Click(hWndTg, 200, 52); // Triple clicks to highlight previous content
    Click(hWndTg, 200, 52);
    Click(hWndTg, 200, 52);
    Backspace(hWndTg); // Clears all content
    Paste(hWndTg); // Paste to overwrite previous content
#endif

    // Finds lowest loading bar
    while (!searchLoaded)
    {
        cx = 1;
        cy = rcTg.bottom - rcTg.top;
        ox = x;
        oy = 0;

        CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

        // Gets lowest loading bar
        for (int i = (cx * cy - 1) * 4; i > -1; i -= 4)
        {
            // Loading bar background's rgb
            if (pBytes[i + 2] == 32 && pBytes[i + 1] == 43 && pBytes[i] == 56)
            {
                x = (i / 4) % cx + ox;
                y = (i / 4) / cx + oy;

                cx = 12;
                cy = 1;
                ox = x - 24;
                oy = y - 12;

                CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

                // Check if "All chats" text appears in loading bar
                for (int i = 0; i < cx * cy * 4; i += 4)
                {
                    // Loading bar background's rgb
                    if (pBytes[i + 2] == 32 && pBytes[i + 1] == 43 && pBytes[i] == 56)
                        ; // Waits
                    else
                    {
                        searchLoaded = TRUE;
                        break;
                    }

                    std::cout << (i / 4) % cx + ox << ", " << (i / 4) / cx + oy << ": " << (int)pBytes[i + 2] << ", " << (int)pBytes[i + 1] << ", " << (int)pBytes[i] << std::endl;
                }

                break;
            }
        }
    }

    Click(hWndTg, x - 4, rcTg.bottom - rcTg.top - 4);

    // DEPRECATED: Checks for result's scrollbar
    if (false) {
        ox = x - 4;

        CaptureWindow(hWndTg, ox, oy, 1, 1, &pBytes[0]);

        // Loading bar background's rgb
        if (pBytes[2] == 32 && pBytes[1] == 43 && pBytes[0] == 56)
            MessageBox(NULL, L"No scroll bar", L"", MB_OK)
            ; // No scrollbar
        else
        {
            // Finds scrollbar
            //return 1;

            MessageBox(NULL, L"Found scroll bar", L"", MB_OK);

            Click(hWndTg, ox + 24, rcTg.bottom - rcTg.top - 24);

            Sleep(5000);

            while (false)
            {
                Sleep(50);

                CaptureWindow(hWndTg, ox, oy, 1, 1, &pBytes[0]);

                if (pBytes[2] == 32 && pBytes[1] == 43 && pBytes[0] == 56)
                    break;
                else
                    PressKey(hWndTg, VK_DOWN, FALSE);
            }
        }
    }

    // DEPRECATED: Finds blue double ticks
    cx = 1;
    cy = rcTg.bottom - rcTg.top - y;
    ox = x - 68;
    oy = y;

    // DEBUG: Replace orignal position with sample
    cx = 1;
    cy = rcTg.bottom - rcTg.top - y - 72;
    ox = x - 14;
    oy = y;

#ifndef _DEBUG
    // Finds any most bottom texts
    cx = 1;
    cy = rcTg.bottom - rcTg.top - y - 4;
    ox = x - 14;
    oy = y;
#endif

    CaptureWindow(hWndTg, ox, oy, cx, cy, &pBytes[0]);

    for (int i = (cx * cy * 4) - 4; i > -1; i -= 4)
    {
        if (r == pBytes[i + 2]
            && g == pBytes[i + 1]
            && b == pBytes[i])
        {
            continue;
        };

        x = (i / 4) % cx + ox;
        y = (i / 4) / cx + oy;
        a = pBytes[i + 3];
        r = pBytes[i + 2];
        g = pBytes[i + 1];
        b = pBytes[i];

        // DEPRECATED: Double tick's rgb = 114, 188, 253
        if (false && r == 114 && g == 188 && b == 253)
        {
            break;
        }

        if (!(r == 23 && g == 33 && b == 43)) // Search list background's rgb
        {
            break;
        }

        std::cout << x << ", " << y << ": " << r << ", " << g << ", " << b << std::endl;
    }

    if (y == oy) // No record found
    {
        return 2;
    }

#ifndef _DEBUG
    Click(hWndTg, x, y);
#endif

    return 0;
}

//
//  FUNCTION: PasteLarkMessage(HWND)
//
//  PURPOSE: Send message in telegram Desktop App programmatically
//
int PasteLarkMessage(HWND hWndLark)
{
    BYTE* pBytes = new BYTE[4];
    RECT rcTg;
    int cx;
    int cy;

    GetAbsoluteRect(hWndLark, &rcTg);
    cx = rcTg.right - rcTg.left;
    cy = rcTg.bottom - rcTg.top;

    // Closes any discussion & reply
    Esc(hWndLark);
    Esc(hWndLark);

    Sleep(500);

    Click(hWndLark, cx / 2, cy - 50);
    Click(hWndLark, cx / 2, cy - 50);
    Click(hWndLark, cx / 2, cy - 50); // Triple clicks to highlight all

    Paste(hWndLark);

    return 0;
}