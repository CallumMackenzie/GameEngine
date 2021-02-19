#include "ModWin.h"
#include <stdexcept>
#include "Input.h"

// Callum Mackenzie

Input* Input::input;

Input* Input::getInput()
{
    if (input == nullptr) {
        input = new Input();
    }
    return input;
}

bool Input::getKeyState(int key)
{
    return GetAsyncKeyState(key);
}

bool Input::getMouseButton(int button)
{
    if (MouseClick[button] != 0 && MouseClick[button] != false)
    {
        return true;
    }
    return false;
}

LPPOINT Input::getCursorPos()
{
    LPPOINT lpt = new tagPOINT();
    if (GetCursorPos(lpt))
    {
        return lpt;
    }
    return nullptr;
}

LPPOINT Input::getHWNDCursorPos(HWND hWnd)
{
    try
    {
        LPPOINT p = getCursorPos();
        if (ScreenToClient(hWnd, p))
        {
            return p;
        }
        return nullptr;
    }
    catch (const std::exception &)
    {
        return nullptr;
    }
}
