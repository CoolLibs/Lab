#pragma once

#include "Clock.h"

class Clock_Realtime : public Clock {
public:
	Clock_Realtime();
	static void Initialize();
	~Clock_Realtime() = default;

	float deltaTime() const override;
	float time() const override;

	void setTime(float newTime) override;
	void update() override;

	void play() override;
	void pause() override;

private:
	Uint64 m_lastTime;
	Uint64 m_currentTime;
	Uint64 m_SDLTimeWhenPaused = 0;
	float m_offsetWithSDLTime; // Allows us to setTime as we wish
	static float SDLPerformanceCounter2Seconds;
};