#pragma once

class Camera;

class CameraState {
public:
	CameraState(Camera& camera)
		: m_camera(camera)
	{}
	~CameraState() = default;

	virtual void update() {};
	virtual void onWheelDown() {};
	virtual void onWheelUp() {};
	virtual void onWheelScroll(float dl) {};
	virtual void onKeyPressed(SDL_Scancode scancode) {};

protected:
	Camera& m_camera;
};