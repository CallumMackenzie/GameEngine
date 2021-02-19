#pragma once

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

protected:
    Engine(); // Engine constructor
};