#pragma once

#include <time.h>
#include "GameObject.h"
#include "Direct2D.h"
#include "Log.h"

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
		static const bool SPRITESHEET_VERTICAL = true;
		static const bool SPRITESHEET_HORIZONTAL = false;

		int frames = 1;
		int frame = 0;
		float frameTime = 0;
		int frameWidth = 0;
		int frameHeight = 0;
		clock_t frameDeltaClock = clock();
		bool spriteSheetDirection = SPRITESHEET_HORIZONTAL;

		void calculateFrame() {
			if (((float)clock() / CLOCKS_PER_SEC) - ((float)frameDeltaClock / CLOCKS_PER_SEC) >= frameTime) {
				frameDeltaClock = clock();
				frame++;
				if (frame >= frames) {
					frame = 0;
				}
			}
		}
	};

	T* renderElement = nullptr;
	float transparency = 1.f;
	FrameData frameData = FrameData();
	// int renderIndex = 0;
};

