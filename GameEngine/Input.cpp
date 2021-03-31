#ifdef _WIN32
#include "ModWin.h"
#endif
#if RENDERER == RENDERER_3D
#include "OpenGL.h"
#endif
#include "Memory.h"
#include <stdexcept>
#include "Vector2.h"
#include "Input.h"

// Callum Mackenzie

Input* Input::input;

#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
Input* Input::getInput()
#else
Input* Input::getInput(GLFWwindow* wnd)
#endif
{
    if (input == nullptr) {
        input = new Input();
    }
#if !(defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS)
    input->win = wnd;
#endif
    return input;
}

Input::~Input()
{
#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
    memory::safe_delete(lpt);
#endif
}

bool Input::getKeyState(int key)
{
#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
    return GetAsyncKeyState(key);
#else
    return glfwGetKey(win, key) == GLFW_PRESS;
#endif
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
#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
    if (GetCursorPos(lpt))
    {
        ret = Vector2(lpt->x, lpt->y);
    }
#else
    double x;
    double y;
    glfwGetCursorPos(win, &x, &y);
    ret.x = x;
    ret.y = y;
#endif
    return ret;
}

#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
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
#else 
Vector2 Input::getHWNDCursorPos()
{
    return getCursorPos();
#endif
}
