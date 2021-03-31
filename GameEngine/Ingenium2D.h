#pragma once

#include "IngeniumConfig.h"
#include <fstream>
#include <sstream>
#include "Memory.h"
#ifdef _WIN32
#include "ModWin.h"
#endif
#if defined(SCRIPT_LUA)
#include "Lua.h"
#endif
#include "WindowClass.h"
#include "Window.h"
#include "OpenGL.h"
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
#include "OpenGLWindow.h"
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
        virtual void start();

#if defined(_WIN32) && RENDERER == RENDERER_2D_WINDOWS
        static LRESULT CALLBACK DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Modified wnd_proc that handles input
#endif
        void createWindow(const char* name, int width, int height);

        virtual void onCreate();
        virtual void onUpdate();
        virtual void onFixedUpdate();
        virtual void onClose();
        virtual ~Ingenium2D(); // Ingenium2D deconstructor to clean up memory leaks

        std::string getFileAsString(std::string fPath);

        bool running = false; // Whether the engine is running or not

    protected:
        virtual void init(); // Initializes the engine
#ifdef SCRIPT_LUA
        virtual void loadToLua();
#endif

    public:
#if RENDERER == RENDERER_2D_WINDOWS
        WindowClass* primeClass = nullptr;
        Direct2DWindow* drwn = nullptr;
#endif
#if RENDERER == RENDERER_3D
        OpenGLWindow* drwn = nullptr;
#endif

    public:
        static Ingenium2D* engine; // Ingenium2D singleton
        Ingenium2D(); // Ingenium2D constructor
    };
}