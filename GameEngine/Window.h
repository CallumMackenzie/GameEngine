#pragma once

// Callum Mackenzie

class Window
{
public:
	~Window();
	void create(); // Creates windows with current set perameters
	void show(); // Shows window
	void crShow(); // Creates and shows window
	HWND getHWND(); // Returns the window handle of the window
	HINSTANCE getHInstance(); // Reutrns the hInstance of the window

	void assignHWND(HWND hwnd);
	bool embedWallpaper(); // Embeds this window in the wallpaper
	bool embedWallpaper(int x_, int y_, int wid, int hei);
	bool isWallpaper(); // Returns whether this window is a wallpaper or not

	static BOOL CALLBACK EnumWindowsProc(HWND wnd, LPARAM lp); // Do not use externally
	static HWND getWallpaperWindow(); // Returns the wallpaper window handle
	void setSize(int width_, int height_); // Sets the size of the window

public:
	int style = 0;           // Prime window style
	int fullscreenStyle = 0; // Style of the full screen window mode
	int opStyle = 0;         // Extended window style
	HMENU hMenu = nullptr;   // Handle to menu or child window
	LPVOID lParam = nullptr; // Window creation data
	int showCode = SW_SHOW;  // Code called when showing window
	bool verbose = false;    // For debugging purposes

protected:
	bool isProgman = false;
	WindowClass* cls = nullptr;    // Window class
	LPCWSTR name = nullptr;        // Window name
	HWND parentWindow = nullptr;   // Parent window
	int x = CW_USEDEFAULT;         // X pos of window
	int y = CW_USEDEFAULT;         // Y pos of window
	int width = CW_USEDEFAULT;     // Width of window
	int height = CW_USEDEFAULT;    // Height of window
	bool shown = false;            // Whether create() has been called
	const char* type = "base";     // Window type
	HINSTANCE hInstance = nullptr; // hInstance
	HWND hWnd = nullptr;           // This HWND window
};

class RootWindow : public Window
{
public:
	RootWindow(HINSTANCE hInstance_, WindowClass* wndcl, LPCWSTR name_, int x_, int y_, int height_, int width_);
	~RootWindow();
	void setFullscreen(); // Sets window to fullscreen
};

class Button : public Window
{
public:
	Button(HINSTANCE hInstance_, HWND parent, LPCWSTR text, int x_, int y_, int height_, int width_); // Creates a new button with the specified attributes
};