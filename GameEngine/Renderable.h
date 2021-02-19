#pragma once

#include "GameObject.h"

class Renderable : GameObject
{
public:
	Renderable(const char* name_, Vector2* pos, Rotation* rot);
};