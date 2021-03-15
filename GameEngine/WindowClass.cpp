// Callum Mackenzie

#include "ModWin.h"
#include "WindowClass.h"

WindowClass::WindowClass(LPCWSTR name_, HINSTANCE hInstance)
{
    name = name_;
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = name;
    wc.hIconSm = nullptr;

    hInst = hInstance;
}
bool WindowClass::setPredefined()
{
    if (registered)
    {
        return false;
    }
    predef = true;
    return predef;
}
void WindowClass::setWindowProc(LRESULT CALLBACK WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
{
    if (!registered && !predef)
    {
        wc.lpfnWndProc = WND_PROC;
    }
}
void WindowClass::registerClass()
{
    if (!registered && !predef)
    {
        RegisterClassEx(&wc);
        registered = true;
    }
}