// ShellExecute.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ShellExecute.h"
//#include "windows.h"
#include "Shellapi.h"

int WINAPI _tWinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine,
                   int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	::ShellExecute(NULL, L"open", lpCmdLine, NULL, NULL, nCmdShow);
	return 0;
}