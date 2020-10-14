#pragma once

#include "CameraState.h"

class CameraState_Translation : public CameraState {
public:
	CameraState_Translation(Camera& camera);
	~CameraState_Translation() = default;

	void update() override;
	void onWheelUp() override;

private:
	glm::vec3 m_initialLookAt;
	glm::vec2 m_initialMousePos; // In centimeters
};