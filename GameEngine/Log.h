#pragma once

#ifdef _WIN32
#include "ModWin.h"
#endif
#include "WindowClass.h"
#include "Window.h"
#include <time.h>

#include <sstream>

// Callum Mackenzie

class Debug
{
public:
	static inline float writeTimeout = 0.1f; // Time, in seconds, between window logs

	static inline std::ostringstream oss;
	static void write();
	static void writeLn();
};