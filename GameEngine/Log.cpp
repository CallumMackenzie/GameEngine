#include "Log.h"

void Debug::write()
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

void Debug::writeLn()
{
	oss << "\n";
	write();
}