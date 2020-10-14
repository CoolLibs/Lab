#include "CameraState_Idle.h"
#include "CameraState_Rotation.h"
#include "CameraState_Translation.h"
#include "Camera.h"
#include "Framework/Input.h"

CameraState_Idle::CameraState_Idle(Camera& camera)
	: CameraState(camera)
{}

void CameraState_Idle::onWheelDown() {
	if (Input::KeyIsDown(SDL_SCANCODE_LSHIFT)){
		m_camera.setState<CameraState_Translation>();
	}
	else {
		m_camera.setState<CameraState_Rotation>();
	}
}

void CameraState_Idle::onWheelScroll(float dl){
	m_camera.m_distToLookAt = std::max(m_camera.m_distToLookAt - dl, 0.00001f);
	m_camera.onTransformChanged();
}