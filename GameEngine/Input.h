#pragma once

// Callum Mackenzie

class Input
{
public:
    static Input* getInput();

public:
    bool getKeyState(int key);
    bool getMouseButton(int button);
    LPPOINT getCursorPos();
    LPPOINT getHWNDCursorPos(HWND hWnd);
    bool MouseClick[20] = { 0 };

private:
    static Input* input;
};