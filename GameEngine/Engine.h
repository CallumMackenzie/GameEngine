#pragma once

#include "ModWin.h"
#include "WindowClass.h"
#include "Window.h"
#include "LinkedList.h"
#include "Direct2D.h"
#include "Direct2DWindow.h"
#include "Input.h"
#include "Renderable.h"
#include "Log.h"
#include "Sprite.h"

// Callum Mackenzie

class Engine
{
public:
    static Engine* getEngine(); // Returns the engine singleton, or creates one if it doesn't exist yet

public:
    void stop(); // Stop the game engine, call deconstructors and perform closing operations.
    static LRESULT CALLBACK DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Modified wnd_proc that handles input
    void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow); // Initializes the engine
    bool running = false; // Whether the engine is running or not
    ~Engine(); // Engine deconstructor to clean up memory leaks

public:
    WindowClass* primeClass = nullptr;
    Direct2DWindow* drwn = nullptr;

private:
    static Engine* engine; // Engine singleton

    LinkedList<GameObject>* gameObjects = new LinkedList<GameObject>();
    LinkedList<Sprite>* sprites = new LinkedList<Sprite>();

private:
    Engine(); // Engine constructor
};