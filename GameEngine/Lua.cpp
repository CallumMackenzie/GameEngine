
#include "Lua.h"
#if defined(_WIN32) && defined(SCRIPT_LUA)
namespace ingenium_lua
{
	lua_State* state = nullptr;

	void initLua()
	{
		OutputDebugStringA("Initializing Lua.\n");

		state = luaL_newstate();
		luaL_openlibs(state);
	}

	void loadFileA(const char* file)
	{
		if (!state) {
			initLua();
		}
		if (luaL_loadfile(state, file)) {
			OutputDebugStringA("Lua file loading failed - ");
			OutputDebugStringA(file);
			OutputDebugStringA(": ");
			OutputDebugStringA(lua_tostring(state, -1));
			OutputDebugStringA("\n");
		}
	}

	bool executeChunk()
	{
		if (!state) {
			initLua();
		}
		if (lua_pcall(state, 0, LUA_MULTRET, 0)) {
			OutputDebugStringA("Lua chunk execution failed: ");
			OutputDebugStringA(lua_tostring(state, -1));
			OutputDebugStringA("\n");
			lua_pop(state, 1);
			return false;
		}
		return true;
	}

	bool executeFunc(const char* function)
	{
		if (!state) {
			initLua();
		}
		lua_getglobal(state, function);
		if (lua_pcall(state, 0, LUA_MULTRET, 0)) {
			OutputDebugStringA("Lua function execution failed: ");
			OutputDebugStringA(lua_tostring(state, -1));
			OutputDebugStringA("\n");
			lua_pop(state, 1);
			return false;
		}
		return true;
	}

	void stopLua()
	{
		if (!state) {
			return;
		}
		lua_close(state);
	}

	std::string getStackTrace() {
		if (!state) {
			return "";
		}
		int top = lua_gettop(state);
		std::string ret ("");
		ret = ret.append("---- Begin Stack ----\nStack size: ").append(std::to_string(top)).append("\n");
		for (int i = top; i >= 1; i--)
		{
			int t = lua_type(state, i);
			switch (t)
			{
			case LUA_TSTRING:
				ret = ret.append(std::to_string(i)).append("-- ").append(lua_tostring(state, t));
				break;
			case LUA_TBOOLEAN:
				ret = ret.append(std::to_string(i)).append("-- ").append(lua_toboolean(state, t) ? "true" : "false");
				break;
			case LUA_TNUMBER:
				{
					float num = lua_tonumber(state, t);
					ret = ret.append(std::to_string(i)).append("-- ").append(std::to_string(num));
				}
				break;
			default:
				ret = ret.append(std::to_string(i)).append("-- ").append(lua_typename(state, t));
				break;
			}
			ret = ret.append("\n");
		}
		ret = ret.append("---- End Stack ----\n");
		return ret;
	}
	luaL_Reg lua_func(const char* name, lua_CFunction cf)
	{
		luaL_Reg lr = luaL_Reg();
		lr.name = name;
		lr.func = cf;
		return lr;
	}
}
#endif