#pragma once

#include <string>

extern "C"
{
#include "lualib/lua.h"
#include "lualib/lauxlib.h"
#include "lualib/lualib.h"
}

#include "ModWin.h"

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
