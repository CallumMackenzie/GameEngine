#pragma once

// Callum Mackenzie

class Time
{
public:
    static Time* getTime();

public:
    int targetFramesPerSecond = 144;
    float targetDeltaTime = 0;
    int deltaTime = 0;
    float deltaMarginOfError = 0;
    float deltaTimeSync = 0;

private:
    static Time* time;
};