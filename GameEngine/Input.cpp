#include "ModWin.h"
#include "Memory.h"
#include <stdexcept>
#include "Vector2.h"
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

Input::~Input()
{
    memory::safe_delete(lpt);
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

Vector2 Input::getCursorPos()
{
    Vector2 ret;
    if (GetCursorPos(lpt))
    {
        ret = Vector2(lpt->x, lpt->y);
    }
    return ret;
}

Vector2 Input::getHWNDCursorPos(HWND hWnd)
{
    Vector2 ret = Vector2();
    if (GetCursorPos(lpt)) {
        if (ScreenToClient(hWnd, lpt))
        {
            ret = Vector2((float)lpt->x, (float)lpt->y);
        }
    }
    return ret;
}
