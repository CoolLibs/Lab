#pragma once

class Clock_Realtime;

class Time {
public:
	static void Initialize();
	static void Update();
	// Time in seconds since the start of the app
	static float time();
	static float deltaTime();

private:
	static std::unique_ptr<Clock_Realtime> s_clock;
};