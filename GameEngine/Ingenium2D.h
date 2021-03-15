#pragma once

#include "IngeniumConfig.h"

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
#include "Shapes.h"
#include "Sprite.h"
#include "Direct2DWindow.h"
#include "Input.h"
#include "Time.h"

// Callum Mackenzie

namespace ingenium2D 
{
    struct Ingenium2D
    {
    public:
        static Ingenium2D* getEngine(); // Returns the engine singleton, or creates one if it doesn't exist yet

        virtual void stop(); // Stop the game engine, call deconstructors and perform closing operations.
        virtual void start(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow);

        static LRESULT CALLBACK DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Modified wnd_proc that handles input
        virtual void onCreate();
        virtual void onUpdate();
        virtual void onFixedUpdate();
        virtual void onClose();
        virtual ~Ingenium2D(); // Ingenium2D deconstructor to clean up memory leaks

        bool running = false; // Whether the engine is running or not


    protected:
        virtual void init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow); // Initializes the engine
#ifdef SCRIPT_LUA
        virtual void loadToLua();
#endif

    public:
        WindowClass* primeClass = nullptr;
        Direct2DWindow* drwn = nullptr;

    protected:
        static Ingenium2D* engine; // Ingenium2D singleton
        Ingenium2D(); // Ingenium2D constructor
    };
}