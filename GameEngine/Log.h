#pragma once

#include <sstream>

// Callum Mackenzie

class Debug
{
public:
	static inline float writeTimeout = 0.1f; // Time, in seconds, between window logs

	static inline std::ostringstream oss;
	static void write();
	static void writeLn();

	static void createDebugWindow(HINSTANCE hInstance); // Creates debug window
	static void writeToWindow(); // Writes oss value to window
	static void destroyDebugWindow(); // Destroys debug window and resources
	static bool windowWriteReady(); // Returns whether enough time has passed to log next data

private:
	static inline WindowClass* guiDebugClass = nullptr;
	static inline RootWindow* guiDebugWindow = nullptr;
	static inline RECT* lpRect = nullptr;
	static inline clock_t clk = clock();
	static inline HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));

	static LRESULT CALLBACK DEBUG_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};