#include <time.h>
#include "Time.h"

// Callum Mackenzie

Time* Time::time;

Time* Time::getTime()
{
    if (time == nullptr) {
        time = new Time();
    }
    return time;
}

Time::Time()
{
    lastClock = clock();
    setFPS(targetFramesPerSecond);
}

int Time::timeSinceLastClock()
{
    return clock() - lastClock;
}

bool Time::nextFrameReady()
{
    if (((float)timeSinceLastClock() / CLOCKS_PER_SEC) >= targetDeltaTime - deltaMarginOfError) {
        deltaTime = timeSinceLastClock();
        lastClock = clock();
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

