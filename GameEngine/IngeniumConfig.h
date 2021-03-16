#pragma once
#include "IngeniumConstants.h"

// #define SCRIPT_LUA 1
// #define INGENIUM_WND_GUI 1

#define RENDERER RENDERER_OPENGL

#define LUA_ENGINE_ENTRY "./Entry.lua"
#define LUA_ENGINE_INIT "init"
#define LUA_ENGINE_UPDATE "onUpdate"
#define LUA_ENGINE_FIXED_UPDATE "onFixedUpdate"
#define LUA_ENGINE_CLOSE "onClose"

#define MAX_OBJ_LINE_CHARS 256