#pragma once

#include "GameObject.h"
#include "Direct2D.h"

template <typename T>
class Renderable : public GameObject
{
	//TODO : Static assert preventing certain template args
public:
	Renderable(const char* name_, Vector2 pos, Rotation rot) : GameObject(name_, pos, rot) 
	{ };
	Renderable(const char* name_, Vector2 pos) : Renderable(name_, pos, Rotation())
	{ };
	Renderable(const char* name_) : Renderable(name_, Vector2(), Rotation())
	{ };
	Renderable(const char* name_, Rotation rot) : Renderable(name_, Vector2(), rot)
	{ };

public:
	T renderElement;
	// int renderIndex = 0;
};
