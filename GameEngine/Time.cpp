#include <time.h>
#include "Time.h"

// Callum Mackenzie

clock_t Time::lastClock;
clock_t Time::lastFixedClock;

int Time::timeSinceLastClock()
{
	return clock() - lastClock;
}

void Time::init()
{
	lastClock = clock();
	lastFixedClock = clock();
	setFPS(targetFramesPerSecond);
	setFixedFPS(targetFixedFramesPerSecond);
}

bool Time::nextFrameReady()
{
	if (((float)timeSinceLastClock() / CLOCKS_PER_SEC) >= targetDeltaTime) {
		deltaTime = timeSinceLastClock();
		lastClock = clock();
		return true;
	}
	return false;
}

bool Time::nextFixedFrameReady()
{
	if (((float)(clock() - lastFixedClock) / CLOCKS_PER_SEC) >= targetFixedDeltaTime) {
		fixedDeltaTime = clock() - lastFixedClock;
		lastFixedClock = clock();
		return true;
	}
	return false;
}

void Time::setFPS(float tFPS)
{
	targetFramesPerSecond = tFPS;
	targetDeltaTime = 1.f / tFPS;
	targetDeltaTime = targetDeltaTime < minDeltaTime ? minDeltaTime : targetDeltaTime;
}

void Time::setFixedFPS(float tFFPS)
{
	targetFixedFramesPerSecond = tFFPS;
	targetFixedDeltaTime = 1.f / tFFPS;
	targetFixedDeltaTime = targetDeltaTime < minDeltaTime ? minDeltaTime : targetDeltaTime;
}

