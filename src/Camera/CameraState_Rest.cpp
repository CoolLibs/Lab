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
		//m_camera.setState<CameraState_Translation>();
	}
	else {
		m_camera.setState<CameraState_Rotation>();
	}
}

void CameraState_Rest::onWheelScroll(float dl){
	m_camera.m_distToLookAt += dl;
	m_camera.onTransformChanged();
}

void CameraState_Rest::onKeyPressed(SDL_Scancode scancode) {
	constexpr float speed = 0.35f;
	if (scancode == SDL_SCANCODE_W) {
		m_camera.position() += speed * m_camera.zAxis();
		m_camera.onTransformChanged();
	}
	else if (scancode == SDL_SCANCODE_S) {
		m_camera.position() -= speed * m_camera.zAxis();
		m_camera.onTransformChanged();
	}
	else if (scancode == SDL_SCANCODE_A) {
		m_camera.position() -= speed * m_camera.xAxis();
		m_camera.onTransformChanged();
	}
	else if (scancode == SDL_SCANCODE_D) {
		m_camera.position() += speed * m_camera.xAxis();
		m_camera.onTransformChanged();
	}
	else if (scancode == SDL_SCANCODE_SPACE) {
		m_camera.position() += speed * glm::vec3(0, 1, 0);
		m_camera.onTransformChanged();
	}
	else if (scancode == SDL_SCANCODE_LSHIFT) {
		m_camera.position() -= speed * glm::vec3(0, 1, 0);
		m_camera.onTransformChanged();
	}
}