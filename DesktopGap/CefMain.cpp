
#include "StdAfx.h"
#include <direct.h>
#include "CefMain.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"

class CefClientApp
	: public CefApp
{
	IMPLEMENT_REFCOUNTING(CefClientApp);
};


CefMain::CefMain(void)
{
	m_fInitialized = false;
	m_fShouldEraseBackground = true;
}

CefMain::~CefMain(void)
{
	CefShutdown();
}

bool CefMain::Initialize(HINSTANCE hInstance)
{
	CefMainArgs mainArgs(hInstance);
	CefRefPtr<CefClientApp> app(new CefClientApp);

	if (CefExecuteProcess(mainArgs, app.get()) >= 0)
		return false;

	CefSettings settings;

	if(!CefInitialize(mainArgs, settings, app.get()))
		return false;

	m_fInitialized = true;
	return true;
}

void CefMain::Shutdown()
{
	if(m_fInitialized)
	{
		m_fInitialized = false;
		CefShutdown();
	}
}

bool CefMain::RunMessageLoop()
{
	CefRunMessageLoop();
	return true;
}

void CefMain::CreateBrowser(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	TCHAR szAppPath[MAX_PATH];
	GetModuleFileName(NULL, szAppPath, MAX_PATH);
	szAppPath[lstrlen(szAppPath) - 4] = 0;
	wcscat_s(szAppPath, L".zip");

	CefWindowInfo info;
	info.SetAsChild(hWnd, rect);

	m_handlers = new CefHandlers(hWnd);
	m_handlers.get()->LoadZip(szAppPath);

	CefBrowserSettings settings;
	//settings.developer_tools_disabled = true;

	CefBrowserHost::CreateBrowser(info, static_cast<CefRefPtr<CefClient>>(m_handlers), "http://invalid.url/index.html", settings);
}

void CefMain::OnClose()
{
	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser.get())
		browser->GetHost()->ParentWindowWillClose();
}

bool CefMain::ShouldEraseBackground()
{
	if(m_fShouldEraseBackground)
	{
		CefRefPtr<CefBrowser> browser = GetBrowser();
		if (browser.get() && browser->GetHost()->GetWindowHandle())
				m_fShouldEraseBackground = false;
	}

	return m_fShouldEraseBackground;
}

void CefMain::OnSize(HWND hWnd, WPARAM wParam)
{
	CefRefPtr<CefBrowser> browser = GetBrowser();
	
	if (wParam != SIZE_MINIMIZED && browser.get())
	{
		CefWindowHandle hWndBrowser = browser->GetHost()->GetWindowHandle();
		
		if (hWndBrowser)
		{
		  RECT rect;
		  GetClientRect(hWnd, &rect);

		  HDWP hdwp = BeginDeferWindowPos(1);
		  hdwp = DeferWindowPos(hdwp, hWndBrowser, NULL,
			rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
			SWP_NOZORDER);
		  EndDeferWindowPos(hdwp);
		}
	}
}

CefRefPtr<CefBrowser> CefMain::GetBrowser()
{
	if (m_handlers.get())
		return m_handlers->m_browser;

	return CefRefPtr<CefBrowser>();
}
