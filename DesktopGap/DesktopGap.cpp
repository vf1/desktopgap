// DesktopGap.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DesktopGap.h"
#include "CefMain.h"
#include "NTray.h"

#define MAX_LOADSTRING	100
#define WM_NOTIFYTRAY	WM_APP+10

WPARAM WM_MAXIMIZEWINDOW = -1;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
CTrayNotifyIcon m_TrayNotify;
BOOL fMinimized;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL				HasRuningCopy();

CefMain cef;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	if(!cef.Initialize(hInstance))
		return FALSE;

	if(HasRuningCopy())
		return 0;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DESKTOPGAP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if(nCmdShow == SW_SHOWMINIMIZED || nCmdShow == SW_SHOWMINNOACTIVE)
	{
		nCmdShow = SW_HIDE;
		fMinimized = TRUE;
	}
	else
	{
		fMinimized = FALSE;
	}

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DESKTOPGAP));

	if (!cef.RunMessageLoop())
	{	
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	cef.Shutdown();

	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESKTOPGAP));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DESKTOPGAP);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, 400, 480, NULL, NULL, hInstance, NULL);

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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	if(WM_MAXIMIZEWINDOW != -1 && message == WM_MAXIMIZEWINDOW)
	{
		message = WM_COMMAND;
		wParam = IDM_SHOW;
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_SHOW:
			if(fMinimized)
			{
				fMinimized = FALSE;
				::ShowWindow(hWnd, SW_SHOW);
			}
			break;
		}
		break;
	case WM_CREATE:
		cef.CreateBrowser(hWnd);
		m_TrayNotify.Create(
			new CWindow(hWnd),
			IDC_DESKTOPGAP, 
			L"", 
			CTrayNotifyIcon::LoadIcon(hInst, MAKEINTRESOURCE(IDI_DESKTOPGAP)), 
			WM_NOTIFYTRAY, 0, TRUE);
		m_TrayNotify.SetDefaultMenuItem(IDM_SHOW, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		if(!cef.ShouldEraseBackground())
			return 0;
		break;
	case WM_SIZE:
		cef.OnSize(hWnd, wParam);
		break;
	case WM_CLOSE:
		cef.OnClose();
		m_TrayNotify.Delete();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SYSCOMMAND:
		if(wParam == SC_CLOSE || wParam == SC_MINIMIZE)
		{
			if(!fMinimized)
			{
				fMinimized = TRUE;
				::ShowWindow(hWnd, SW_HIDE);
			}
			return 0;
		}
		break;
	case WM_NOTIFYTRAY:
		m_TrayNotify.OnTrayNotification(wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL HasRuningCopy()
{
	TCHAR szAppPath[MAX_PATH];
	GetModuleFileName(NULL, szAppPath, MAX_PATH);
	for(int i=0; szAppPath[i]!=0; i++)
		if(szAppPath[i] == '\\')
			szAppPath[i] = '-';

	WM_MAXIMIZEWINDOW = ::RegisterWindowMessage(szAppPath);
	::CreateMutex(NULL, FALSE, szAppPath);

	if(GetLastError() != ERROR_SUCCESS)
	{
		PostMessage(HWND_BROADCAST, WM_MAXIMIZEWINDOW, 0, 0);
		return TRUE;
	}

	return FALSE;
}