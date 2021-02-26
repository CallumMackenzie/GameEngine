#include "Log.h"

void Debug::write()
{
	try
	{
		OutputDebugStringA(oss.str().c_str());
		oss.str("");
		oss.clear();
	}
	catch (const std::exception&)
	{
		return;
	}
}

void Debug::writeLn()
{
	oss << "\n";
	write();
}

LRESULT Debug::DEBUG_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Debug::createDebugWindow(HINSTANCE hInstance) {
	guiDebugClass = new WindowClass(L"Debug WC", hInstance);
	guiDebugClass->setWindowProc(DEBUG_WND_PROC);
	guiDebugClass->registerClass();

	int wid = 400;
	int hei = 300;

	guiDebugWindow = new RootWindow(hInstance, guiDebugClass, L"Debug", CW_USEDEFAULT, CW_USEDEFAULT, hei, wid);
	guiDebugWindow->create();
	guiDebugWindow->show();
	lpRect = new RECT();
	lpRect->left = 10;
	lpRect->top = 10;
	lpRect->right = wid - 10;
	lpRect->bottom = hei - 10;
}

void Debug::writeToWindow()
{
	if (lpRect != nullptr) {
		FillRect(GetDC(guiDebugWindow->getHWND()), lpRect, hBrush);
		DrawTextA(GetDC(guiDebugWindow->getHWND()), oss.str().c_str(), -1, lpRect, DT_LEFT | DT_TOP);
	}
	oss.str("");
	oss.clear();
	InvalidateRect(guiDebugWindow->getHWND(), NULL, FALSE);
}

void Debug::destroyDebugWindow()
{
	delete guiDebugWindow;
	delete guiDebugClass;
	delete lpRect;
}

bool Debug::windowWriteReady()
{
	if (((float)clock() / CLOCKS_PER_SEC) - ((float)clk / CLOCKS_PER_SEC) >= writeTimeout) {
		clk = clock();
		return true;
	}
	return false;
}