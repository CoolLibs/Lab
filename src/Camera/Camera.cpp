#include "Camera.h"
#include "CameraState_Rest.h"

#include "glm/gtc/matrix_transform.hpp"
#include "Framework/RenderState.h"
#include "Framework/Input.h"
#include "Utility/Math.h"

Camera::Camera() {
	setState<CameraState_Rest>();
	onTransformChanged();
	onProjectionChanged();
}

void Camera::onTransformChanged() {
	m_transformMatrix = glm::translate(glm::rotate(glm::rotate(glm::mat4(1.0f), m_angleUp, glm::vec3(1, 0, 0)), m_angleGround, glm::vec3(0, 1, 0)), m_position);
	//m_transformMatrix = glm::rotate(m_transformMatrix, m_angleUp, xAxis());
	m_inverseTransformMatrix = glm::inverse(m_transformMatrix);
}

void Camera::onProjectionChanged() {
	// Nothing to do at the moment
}

//Ray Camera::getRayGoingThroughMouse() {
//	glm::vec3 pos = position();
//	glm::vec3 mousePos = glm::unProject(glm::vec3(Input::MousePositionInPixels(), 0.0f), viewMatrix(), getProjMatrix(), glm::vec4(0.0f, 0.0f, Locate::renderer().getWidth(), Locate::renderer().getHeight()));
//	glm::vec3 dir = glm::normalize(mousePos - pos);
//	return Ray(pos, dir);
//}