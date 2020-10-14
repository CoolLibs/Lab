#include "CameraState_Rest.h"
#include "CameraState_Rotation.h"
#include "CameraState_Translation.h"
#include "Camera.h"
#include "Framework/Input.h"

CameraState_Rest::CameraState_Rest(Camera& camera)
	: CameraState(camera)
{}

void CameraState_Rest::onWheelDown() {
	if (Input::KeyIsDown(SDL_SCANCODE_LSHIFT)){
		m_camera.setState<CameraState_Translation>();
	}
	else {
		m_camera.setState<CameraState_Rotation>();
	}
}

void CameraState_Rest::onWheelScroll(float dl){
	m_camera.m_distToLookAt = std::max(m_camera.m_distToLookAt - dl, 0.00001f);
	m_camera.onTransformChanged();
}