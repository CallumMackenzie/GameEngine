#pragma once

// Callum Mackenzie

class Time
{
public:
	static void init();

	static bool nextFrameReady();
	static bool nextFixedFrameReady();

	static void setFPS(float tFPS);
	static void setFixedFPS(float tFFPS);

public:
	static inline float targetFramesPerSecond = 200;
	static inline float targetDeltaTime = 0;
	static inline float deltaTime = 0;
	static inline const double maxFPS = 500;
	static inline const double minDeltaTime = 1.0 / maxFPS;

	static inline float targetFixedFramesPerSecond = 40;
	static inline float targetFixedDeltaTime = 0;
	static inline float fixedDeltaTime = 0;

	static clock_t lastClock;
	static clock_t lastFixedClock;
};