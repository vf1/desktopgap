#pragma once

#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "unzip.h"

class CefHandlers
	: public CefClient
	, public CefRequestHandler
	, public CefContextMenuHandler
	, public CefLoadHandler
	, public CefLifeSpanHandler
	, public CefDisplayHandler
{
public:
	HZIP m_hz;
	CefRefPtr<CefBrowser> m_browser;
	HWND m_hHostWnd;
	BOOL m_fTitleAssigned;

	CefHandlers(HWND hHostWnd);
	~CefHandlers();

	void LoadZip(LPCTSTR fileName);
	bool GetZippedFile(std::string fileName, PVOID& pData, int& length);
	CefRefPtr<CefResourceHandler> GetFileHandler(std::string fileName);

	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override;
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;

	virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) override;
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override;
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings) override;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;

	IMPLEMENT_REFCOUNTING(CefClientApp);
};

