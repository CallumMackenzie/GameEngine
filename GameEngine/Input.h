#pragma once

// Callum Mackenzie

#include "IngeniumConfig.h"
#include "OpenGL.h"

class Input
{
public:
#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
    static Input* getInput();
#else
    static Input* getInput(GLFWwindow* wnd);
#endif
    static Input* input; // DO NOT USE

    ~Input();
    bool getKeyState(int key);
    bool getMouseButton(int button);
    Vector2 getCursorPos();
#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
    Vector2 getHWNDCursorPos(HWND hWnd);
#else
    Vector2 getHWNDCursorPos();
#endif
    bool MouseClick[20] = { 0 };

private:
#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
    LPPOINT lpt = new tagPOINT();
#else
    GLFWwindow* win = nullptr;
#endif
};