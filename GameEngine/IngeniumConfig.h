#pragma once
#include "IngeniumConstants.h"

// #define SCRIPT_LUA 1
// #define INGENIUM_WND_GUI 1

// RENDERER_DIRECT2D // RENDERER_OPENGL

#define RENDERER RENDERER_OPENGL

#define SHADER_LIGHTS_STRUCT_NAME "Lights"
#define SHADER_LIGHTS_AMBIENT FRAG_SHADER_LIGHTS_STRUCT_NAME ".ambientIntensity"
#define SHADER_LIGHTS_NUMBER "numLights"

#define LUA_ENGINE_ENTRY "./Entry.lua"
#define LUA_ENGINE_INIT "onCreate"
#define LUA_ENGINE_UPDATE "onUpdate"
#define LUA_ENGINE_FIXED_UPDATE "onFixedUpdate"
#define LUA_ENGINE_CLOSE "onClose"

#define MAX_OBJ_LINE_CHARS 256