#pragma once


#include "GameObject.h"
#include "Direct2D.h"

template <typename T>
class Renderable : GameObject
{
	//TODO : Static assert preventing certain template args
public:
	Renderable(const char* name_, Vector2* pos, Rotation* rot) : GameObject(name_, pos, rot) 
	{ };
	Renderable(const char* name_, Vector2* pos) : Renderable(name_, pos, new Rotation())
	{ };
	Renderable(const char* name_) : Renderable(name_, new Vector2(), new Rotation())
	{ };
	Renderable(const char* name_, Rotation* rot) : Renderable(name_, new Vector2(), rot)
	{ };

public:
	T renderElement;
};