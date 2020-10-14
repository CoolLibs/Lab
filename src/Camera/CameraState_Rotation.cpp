#include "CameraState_Rotation.h"
#include "CameraState_Rest.h"
#include "Camera.h"
#include "Framework/Input.h"

CameraState_Rotation::CameraState_Rotation(Camera& camera)
	: CameraState(camera),
	  m_initialAngleGround(camera.angleGround()),
	  m_initialAngleUp    (camera.angleUp()),
	  m_initialMousePos(Input::MouseInCentimeters())
{}

void CameraState_Rotation::update() {
	glm::vec2 mouseDL = Input::MouseInCentimeters() - m_initialMousePos;
	mouseDL.y *= -1.0f;
	m_camera.angleGround() = m_initialAngleGround + mouseDL.x * 0.0001f;
	m_camera.angleUp()     = m_initialAngleUp     + mouseDL.y * 0.0001f;
	//m_camera.m_sphereCoords.angleGround() = m_initialAngleGround + mouseDL.x * glm::two_pi<float>() * 0.0005f / 2.5f;
	//m_camera.m_sphereCoords.angleUp()     = m_initialAngleUp     + mouseDL.y * glm::two_pi<float>() * 0.0005f / 2.5f;
	////
	m_camera.onTransformChanged();
}

void CameraState_Rotation::onWheelUp() {
	m_camera.setState<CameraState_Rest>();
}