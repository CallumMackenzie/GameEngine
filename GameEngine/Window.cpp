#include "ModWin.h"
#include "WindowClass.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "Window.h"

// Callum Mackenzie

Window::~Window()
{
	InvalidateRect(hWnd, 0, TRUE);
	DestroyWindow(hWnd);
}
void Window::create()
{
	if (type == "base")
	{
		hWnd = CreateWindowEx(
			opStyle,
			cls->name,
			name,
			style,
			x, y,
			width, height,
			parentWindow,
			hMenu,
			hInstance,
			lParam);
	}
	else if (type == "button")
	{
		hWnd = CreateWindow(
			L"BUTTON",
			name,
			WS_TABSTOP | WS_VISIBLE | WS_CHILD,
			x, y,
			width, height,
			parentWindow,
			hMenu,
			(HINSTANCE)GetWindowLongPtr(parentWindow, GWLP_HINSTANCE),
			lParam);
	}
	if (verbose)
	{
		//global::print(type);
	}
	shown = true;
}
void Window::show()
{
	ShowWindow(hWnd, showCode);
	shown = true;
}
void Window::crShow()
{
	create();
	show();
}
HWND Window::getHWND()
{
	return hWnd;
}
HINSTANCE Window::getHInstance()
{
	return hInstance;
}

void Window::assignHWND(HWND hwnd)
{
	hWnd = hwnd;
}

BOOL CALLBACK Window::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	HWND p = FindWindowEx(hwnd, NULL, L"SHELLDLL_DefView", NULL);
	HWND* ret = (HWND*)lParam;

	if (p)
	{
		// Gets the WorkerW Window after the current one.
		*ret = FindWindowEx(NULL, hwnd, L"WorkerW", NULL);
	}
	return true;
}

HWND Window::getWallpaperWindow()
{
	HWND progman = FindWindow(L"Progman", NULL);

	SendMessageA(progman, 0x052C, 0xD, 0);
	SendMessageA(progman, 0x052C, 0xD, 1);

	HWND wallpaper_hwnd = nullptr;
	EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_hwnd);

	if (!wallpaper_hwnd)
	{
		SendMessageA(progman, 0x052C, 0, 0);
		EnumWindows(EnumWindowsProc, (LPARAM)&wallpaper_hwnd);
	}

	if (!wallpaper_hwnd)
	{
		wallpaper_hwnd = progman;
	}

	return wallpaper_hwnd;
}

bool Window::embedWallpaper()
{
	return embedWallpaper(x, y, width, height);
}

bool Window::embedWallpaper(int x_, int y_, int wid, int hei)
{
	x = x_;
	y = y_;
	width = wid;
	height = hei;
	HWND wallpaper = getWallpaperWindow();

	style &= ~(
		WS_CAPTION |
		WS_THICKFRAME |
		WS_SYSMENU |
		WS_MAXIMIZEBOX |
		WS_MINIMIZEBOX
		);

	style |= WS_CHILD;

	if (!SetWindowLongA(hWnd, GWL_STYLE, style)) {
		return false;
	}

	RECT* hWndRect = new RECT();
	if (!GetWindowRect(hWnd, hWndRect)) {
		delete hWndRect;
		return false;
	};
	setSize(wid, hei);

	SetParent(hWnd, wallpaper);
	MapWindowPoints(0, hWnd, (LPPOINT)hWndRect, 2);

	ShowWindow(wallpaper, SW_SHOW);
	ShowWindow(hWnd, SW_SHOW);
	delete hWndRect;

	isProgman = true;
	return true;
}

bool Window::isWallpaper()
{
	return isProgman;
}

RootWindow::RootWindow(HINSTANCE hInstance_, WindowClass* wndcl, LPCWSTR name_, int x_, int y_, int height_, int width_)
{
	name = name_;
	cls = wndcl;
	hInstance = hInstance_;
	x = x_;
	y = y_;
	height = height_;
	width = width_;
	style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX;
}
RootWindow::~RootWindow()
{
	DestroyWindow(hWnd);
}
void RootWindow::setFullscreen()
{
	MONITORINFO mi = { sizeof(mi) };

	WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };
	//DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (GetWindowPlacement(hWnd, &g_wpPrev) && GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi))
	{
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_VISIBLE | fullscreenStyle & ~WS_OVERLAPPEDWINDOW);
		width = mi.rcMonitor.right - mi.rcMonitor.left;
		height = mi.rcMonitor.bottom - mi.rcMonitor.top;
		x = mi.rcMonitor.left;
		y = mi.rcMonitor.top;
		SetWindowPos(hWnd, HWND_TOP,
			x, y,
			width,
			height,
			SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	}
}
void Window::setSize(int width_, int height_)
{
	width = width_;
	height = height_;
	if (shown)
	{
		bool menu_ = false;
		if (hMenu != nullptr)
		{
			menu_ = true;
		}
		RECT wrect;
		wrect.left = 0;
		wrect.top = 0;
		wrect.right = width_;
		wrect.bottom = height_;
		AdjustWindowRectEx(&wrect, style, menu_, 0);
		SetWindowPos(hWnd, HWND_TOP, wrect.left, wrect.top, wrect.right, wrect.bottom, SWP_DRAWFRAME);
	}
}

Button::Button(HINSTANCE hInstance_, HWND parent, LPCWSTR text, int x_, int y_, int height_, int width_)
{
	name = text;
	cls = new WindowClass(L"BUTTON", hInstance);
	hInstance = hInstance_;
	x = x_;
	y = y_;
	height = height_;
	width = width_;
	style = WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	parentWindow = parent;
	type = "button";
}