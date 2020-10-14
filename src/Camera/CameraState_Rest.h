#pragma once

#include "CameraState.h"

class CameraState_Rest : public CameraState {
public:
	CameraState_Rest(Camera& camera);
	~CameraState_Rest() = default;

	void onWheelDown() override;
	void onWheelScroll(float dl) override;
	void onKeyPressed(SDL_Scancode scancode) override;
};