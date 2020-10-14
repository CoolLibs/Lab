#pragma once

#include "CameraState.h"

class CameraState_Idle : public CameraState {
public:
	CameraState_Idle(Camera& camera);
	~CameraState_Idle() = default;

	void onWheelDown() override;
	void onWheelScroll(float dl) override;
};