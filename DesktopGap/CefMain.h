#pragma once

#include "CefHandlers.h"

class CefMain
{
protected:
	bool m_fInitialized;
	bool m_fShouldEraseBackground;
	CefRefPtr<CefHandlers> m_handlers;

	CefRefPtr<CefBrowser> GetBrowser();

public:
	CefMain(void);
	~CefMain(void);
	bool Initialize(HINSTANCE hInstance);
	void Shutdown();
	bool RunMessageLoop();
	void CreateBrowser(HWND hWnd);
	bool ShouldEraseBackground();
	void OnClose();
	void OnSize(HWND hWnd, WPARAM wParam);
};

