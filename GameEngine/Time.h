#pragma once

// Callum Mackenzie

class Time
{
public:
    static Time* getTime();

    Time();
    bool nextFrameReady();
    bool nextFixedFrameReady();

    int timeSinceLastClock();
    void setFPS(float tFPS);
    void setFixedFPS(float tFFPS);

public:
    float targetFramesPerSecond = 200;
    float targetDeltaTime = 0;
    float deltaTime = 0;
    const double maxFPS = 500;
    const double minDeltaTime = 1.0 / maxFPS;

    float targetFixedFramesPerSecond = 40;
    float targetFixedDeltaTime = 0;
    float fixedDeltaTime = 0;

private:
    static Time* time;

private:
    clock_t lastClock;
    clock_t lastFixedClock;
};