#include "StdAfx.h"
#include "CefHandlers.h"
#include "Shellapi.h"

CefHandlers::CefHandlers(HWND hHostWnd)
{
	m_hHostWnd = hHostWnd;
	m_fTitleAssigned = FALSE;
}

CefHandlers::~CefHandlers()
{
	CloseZip(m_hz);
}

//---------------------------------------------------------------------------------------------------[ What THIS handle ]

CefRefPtr<CefRequestHandler> CefHandlers::GetRequestHandler()
{
	return this;
}

CefRefPtr<CefContextMenuHandler> CefHandlers::GetContextMenuHandler()
{
	return this;
}

CefRefPtr<CefLifeSpanHandler> CefHandlers::GetLifeSpanHandler()
{
	return this;
}

CefRefPtr<CefDisplayHandler> CefHandlers::GetDisplayHandler()
{
	return this;
}

//---------------------------------------------------------------------------------------------------[ Request Handler ]

CefRefPtr<CefResourceHandler> CefHandlers::GetResourceHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request)
{
	std::string url = request->GetURL();
	std::string special = "http://invalid.url/";

	if(url.find(special) == 0)
	{
		int question = url.find_last_of('?');
		if(question >= 0)
			url = url.substr(0, question);

		return GetFileHandler(url.substr(special.length()));
	}

	return NULL;
}

//---------------------------------------------------------------------------------------------------[ Context Menu ]

void CefHandlers::OnBeforeContextMenu(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model)
{
	for(int i=model.get()->GetCount()-1; i>=0; i--)
	{
		int commandId = model.get()->GetCommandIdAt(i);
		if(commandId < 110 || commandId > 116)
			model.get()->RemoveAt(i);
	}
}

//---------------------------------------------------------------------------------------------------[ Popup ]

bool CefHandlers::OnBeforePopup(
	CefRefPtr<CefBrowser> parentBrowser,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	const CefString& url,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings)
{
	::ShellExecute(NULL, NULL, url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
}

//---------------------------------------------------------------------------------------------------[ Title Change ]

void CefHandlers::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
	if(!m_fTitleAssigned)
	{
		m_fTitleAssigned = TRUE;
		::SetWindowText(m_hHostWnd, title.c_str());
	}
}

//---------------------------------------------------------------------------------------------------[ Browser ]

void CefHandlers::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	if (!m_browser.get())
		m_browser = browser;
}

//---------------------------------------------------------------------------------------------------[ ZIP Helpers ]

void CefHandlers::LoadZip(LPCTSTR fileName)
{
	m_hz = OpenZip(fileName, NULL);
}

bool CefHandlers::GetZippedFile(std::string fileName, PVOID& pData, int& length)
{
	USES_CONVERSION;

	int i; 
	ZIPENTRY ze;

	if(FindZipItem(m_hz, A2T(fileName.c_str()), true, &i, &ze) == ZR_OK)
	{
		length = ze.unc_size;
		pData = new char[length];
		UnzipItem(m_hz, i, pData, length);

		return true;
	}

	return false;
}

CefRefPtr<CefResourceHandler> CefHandlers::GetFileHandler(std::string fileName)
{
	PVOID pData;
	int length;

	if(GetZippedFile(fileName, pData, length))
	{
		CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForData(pData, length);

		return new CefStreamResourceHandler("text/html", stream);
	}
	else
	{
		return new CefStreamResourceHandler(404, "text/html", CefResponse::HeaderMap(), CefStreamReader::CreateForData(new char[1], 1));
	}
}

//---------------------------------------------------------------------------------------------------[ end ]
