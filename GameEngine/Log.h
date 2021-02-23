#pragma once

#include "ModWin.h"
#include <sstream>

// Callum Mackenzie

struct Debug
{
	static inline std::ostringstream oss;

	static inline void write ()
	{
		try
		{
			OutputDebugStringA(oss.str().c_str());
			oss.str("");
			oss.clear();
		}
		catch (const std::exception&)
		{
			return;
		}
	}

	static inline void writeLn()
	{	
		oss << "\n";
		write();
	}
};