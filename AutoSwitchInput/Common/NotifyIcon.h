#pragma once

#include <windows.h>
#pragma comment(lib, "Shell32.lib")


#define WM_TRAYMOUSEMESSAGE  (WM_USER + 1024)


struct NotifyIcon
{

	inline static NOTIFYICONDATA nID = {};

	static void CreateTrayIcon(HWND hWnd)
	{
		nID = {};

		nID.hIcon = (HICON)LoadImage(NULL, L"Assets/Logo.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		wcscpy_s(nID.szTip, L"AutoSwitchInput");
		nID.hWnd = hWnd;
		nID.uID = 1;
		nID.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_SHOWTIP;
		nID.uCallbackMessage = WM_TRAYMOUSEMESSAGE;
		nID.uVersion = NOTIFYICON_VERSION_4;


		Shell_NotifyIcon(NIM_SETVERSION, &nID);
		Shell_NotifyIcon(NIM_ADD, &nID);


	}

	static void RemoveTrayIcon() {

		Shell_NotifyIcon(NIM_DELETE, &nID);
	}

};
