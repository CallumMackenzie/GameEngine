#include <time.h>
#include "Time.h"

// Callum Mackenzie

clock_t Time::lastClock;
clock_t Time::lastFixedClock;

void Time::init()
{
	lastClock = clock();
	lastFixedClock = clock();
	setFPS(targetFramesPerSecond / 1000);
	setFixedFPS(targetFixedFramesPerSecond / 1000);
}

bool Time::nextFrameReady()
{
	float dt = ((float)(clock() - lastClock) / CLOCKS_PER_SEC);
	if (dt >= targetDeltaTime) {
		deltaTime = dt;
		lastClock = clock();
		return true;
	}
	return false;
}

bool Time::nextFixedFrameReady()
{
	float fdt = ((float)(clock() - lastFixedClock) / CLOCKS_PER_SEC);
	if (fdt >= targetFixedDeltaTime) {
		fixedDeltaTime = fdt;
		lastFixedClock = clock();
		return true;
	}
	return false;
}

void Time::setFPS(float tFPS)
{
	targetFramesPerSecond = tFPS * 1000;
	targetDeltaTime = 1.f / tFPS;
	targetDeltaTime = targetDeltaTime;
}

void Time::setFixedFPS(float tFFPS)
{
	targetFixedFramesPerSecond = tFFPS * 1000;
	targetFixedDeltaTime = 1.f / tFFPS;
	targetFixedDeltaTime = targetDeltaTime;
}

