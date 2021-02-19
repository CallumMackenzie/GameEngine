#pragma once

#include "ModWin.h"
#include <sstream>
#include <stdarg.h>

// Callum Mackenzie

template <typename T>
struct Debug
{
	static void Log(T const& ch) {
		try
		{
			std::ostringstream oss;
			oss << ch;
			OutputDebugStringA(oss.str().c_str());
		}
		catch (const std::exception&)
		{
			return;
		}
	};
	static void LogLine(T const& ch)
	{
		try
		{
			std::ostringstream oss;
			oss << ch;
			oss << '\n';
			OutputDebugStringA(oss.str().c_str());
		}
		catch (const std::exception&)
		{
			return;
		}
	};
};