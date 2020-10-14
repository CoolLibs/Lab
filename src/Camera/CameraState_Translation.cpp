#include "CameraState_Translation.h"
#include "CameraState_Rest.h"
#include "Camera.h"
#include "Framework/Input.h"

CameraState_Translation::CameraState_Translation(Camera& camera)
	: CameraState(camera),
	  //m_initialLookedAtPoint(camera.getLookedAtPoint()),
	  m_initialMousePos(Input::MouseInCentimeters())
{}

void CameraState_Translation::update() {
	//glm::vec2 mouseDL = m_initialMousePos - Input::MouseInCentimeters();
	//m_camera.setLookedAtPoint(m_initialLookedAtPoint + (m_camera.xAxis()*mouseDL.x + m_camera.yAxis()*mouseDL.y) * 0.007f * m_camera.m_sphereCoords.getRadius());
	//m_camera.onTransformChanged();
}

void CameraState_Translation::onWheelUp() {
	m_camera.setState<CameraState_Rest>();
}