#pragma once
#include "IngeniumConstants.h"

// #define SCRIPT_LUA 1
// #define INGENIUM_WND_GUI 1

// RENDERER_DIRECT2D // RENDERER_OPENGL

#define RENDERER RENDERER_OPENGL

#define SHADER_LIGHTING_COLOUR "lightColour"
#define SHADER_LIGHTING_INTENSITY "lightIntensity"
#define SHADER_LIGHTING_POSITION "lightPosition"
#define SHADER_LIGHTING_ROTATION "lightRotation"

#define LUA_ENGINE_ENTRY "./Entry.lua"
#define LUA_ENGINE_INIT "onCreate"
#define LUA_ENGINE_UPDATE "onUpdate"
#define LUA_ENGINE_FIXED_UPDATE "onFixedUpdate"
#define LUA_ENGINE_CLOSE "onClose"

#define MAX_OBJ_LINE_CHARS 256