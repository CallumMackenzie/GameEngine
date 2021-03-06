#pragma once

// Callum Mackenzie

extern "C" {
    struct WindowClass
    {
        WindowClass(LPCWSTR name_, HINSTANCE hInstance); // WindowClass constructor. Sets default options for the created window class
        bool setPredefined(); // Sets this window class as a pre-defined one
        void setWindowProc(LRESULT CALLBACK WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)); // Sets the WndProc for the windows in this class
        void registerClass(); // Registers the class to be used in windows

        LPCWSTR name = nullptr; // The name of the window class used to identify it
        WNDCLASSEX wc = { 0 }; // The WindowClass structure that is used to set the properties of it
        bool predef = false; // Whether this WindowClass is predefined or not
        bool registered = false; // Whether this class has been registered yet
    };
}