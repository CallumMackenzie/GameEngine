#pragma once

#include "GameObject.h"
#include "Direct2D.h"

template <typename T>
class Renderable : public GameObject
{
	//TODO : Static assert preventing certain template args
public:
	inline Renderable(const char* name_, Vector2 pos, Rotation rot, T* rElement) : GameObject(name_, pos, rot)
	{
		renderElement = rElement;
	};

	Renderable(const char* name_, Vector2 pos, Rotation rot) 
		: GameObject(name_, pos, rot)
	{ };
	Renderable(const char* name_, Vector2 pos) 
		: Renderable(name_, pos, Rotation())
	{ };
	Renderable(const char* name_) 
		: Renderable(name_, Vector2(), Rotation())
	{ };
	Renderable(const char* name_, Rotation rot) 
		: Renderable(name_, Vector2(), rot)
	{ };

	Renderable(const char* name_, Vector2 pos, T* rElement) 
		: Renderable(name_, pos, Rotation(), rElement)
	{ };
	Renderable(const char* name_, T* rElement) 
		: Renderable(name_, Vector2(), Rotation(), rElement)
	{ };
	Renderable(const char* name_, Rotation rot, T* rElement) 
		: Renderable(name_, Vector2(), rot, rElement)
	{ };

public:
	struct FrameData {
		int frames = 1;
		int frame = 0;
		int frameTime = 0;
		int frameWidth = 0;
		int frameHeight = 0;

		void calculateFrame() {
			frame++;
		}
	};

	T* renderElement = nullptr;
	float transparency = 1.f;
	FrameData frameData = FrameData();
	// int renderIndex = 0;
};
