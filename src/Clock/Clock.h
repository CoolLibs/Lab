#pragma once

class Clock {
public:
	Clock() = default;
	virtual ~Clock() = default;

	virtual float deltaTime() const = 0;
	virtual float time() const = 0;

	virtual void setTime(float newTime) = 0;
	virtual void update() = 0;
};