#pragma once

#ifdef _WIN32
#include "ModWin.h"
#endif

#define MEM_COUNTER \
int memManagementTracker = 0;\
void* operator new (size_t size)\
{\
	memManagementTracker++;\
	return malloc(size);\
}\
void operator delete(void* p)\
{\
	memManagementTracker--;\
	free(p);\
}\
void printAllocationInfo()\
{\
	OutputDebugStringA("Objects not deallocated: ");\
	char mem[10] = { 0 };\
	sprintf_s(mem, sizeof(mem), "%d", memManagementTracker);\
	OutputDebugStringA(mem);\
	OutputDebugStringA("\n");\
}

namespace memory
{
	template<typename T>
	static inline void safe_delete(T*& a)
	{
		if (&a == nullptr)
			return;
		delete a;
		a = NULL;
	}
};