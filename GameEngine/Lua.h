#pragma once

#include "ModWin.h"
#include <string>

extern "C"
{
#include "lualib/include/lua.h"
#include "lualib/include/lauxlib.h"
#include "lualib/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "lualib/lua54.lib")
#endif

namespace ingenium_lua {
#define LUA_ENGINE_ENTRY "./Entry.lua"
#define LUA_ENGINE_INIT "init"
#define LUA_ENGINE_UPDATE "onUpdate"
#define LUA_ENGINE_FIXED_UPDATE "onFixedUpdate"
#define LUA_ENGINE_CLOSE "onClose"

	extern lua_State* state;

	extern "C"
	{
		void initLua();
		void loadFile(const char* file);
		bool executeChunk();
		bool executeFunc(const char* function);
		void stopLua();
		std::string getStackTrace();
	}
}
