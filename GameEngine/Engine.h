#pragma once

#define SCRIPT_LUA 1

#include "Memory.h"
#include "ModWin.h"
#if defined(SCRIPT_LUA)
#include "Lua.h"
#endif
#include "WindowClass.h"
#include "Window.h"
#include "Log.h"
#include "StringCon.h"
#include "LinkedList.h"
#include "Vector.h"
#include "Vector2.h"
#include "Rotation.h"
#include "Physics2D.h"
#include "Renderable.h"
#include "Sprite.h"
#include "Direct2DWindow.h"
#include "Input.h"
#include "Time.h"

// Callum Mackenzie

using namespace ingenium2D;

struct Engine
{
public:
    static Engine* getEngine(); // Returns the engine singleton, or creates one if it doesn't exist yet

public:
    void stop(); // Stop the game engine, call deconstructors and perform closing operations.
    static LRESULT CALLBACK DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Modified wnd_proc that handles input
    void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow); // Initializes the engine
    void onUpdate();
    void onClose();
    bool running = false; // Whether the engine is running or not
    ~Engine(); // Engine deconstructor to clean up memory leaks

#if defined(SCRIPT_LUA)
    void loadToLua();
#endif

public:
    WindowClass* primeClass = nullptr;
    Direct2DWindow* drwn = nullptr;

private:
    static Engine* engine; // Engine singleton

    // LinkedList* gameObjects = new LinkedList();

private:
    Engine(); // Engine constructor
};