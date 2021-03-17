#pragma once

// Callum Mackenzie

#include "IngeniumConfig.h"
#include "OpenGL.h"

class Input
{
public:
    static Input* getInput();
    static Input* input; // DO NOT USE

    ~Input();
    bool getKeyState(int key);
    bool getMouseButton(int button);
    Vector2 getCursorPos();
    Vector2 getHWNDCursorPos(HWND hWnd);
    bool MouseClick[20] = { 0 };

private:
    LPPOINT lpt = new tagPOINT();
};