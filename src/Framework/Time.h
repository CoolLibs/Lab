#pragma once

class Clock;

class Time {
public:
	static void Initialize();
	static void Update();
	static void ImGui();
	// In seconds
	static float time();
	static float deltaTime();
	// In seconds
	static void setTime(float t);

private:
	static std::unique_ptr<Clock> m_clock;
};