#pragma once

// Callum Mackenzie

class Time
{
public:
    static Time* getTime();

    Time();
    int timeSinceLastClock();
    bool nextFrameReady();
    void setFPS(float tFPS);

public:
    float targetFramesPerSecond = 200;
    float targetDeltaTime = 0;
    float deltaTime = 0;
    float deltaMarginOfError = 0;
    const double maxFPS = 500;
    const double minDeltaTime = 1.0 / maxFPS;

private:
    static Time* time;

private:
    clock_t lastClock;
};