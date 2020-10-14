#include "Camera.h"
#include "CameraState_Idle.h"

#include "Utility/Math.h"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/matrix_access.hpp>

Camera::Camera() {
	setState<CameraState_Idle>();
	onTransformChanged();
}

void Camera::onTransformChanged() {
	glm::vec3 posRelToLookAt = glm::vec3(
		cos(m_angleUp) * cos(m_angleGround),
		sin(m_angleUp),
		cos(m_angleUp) * sin(m_angleGround)
	) * m_distToLookAt;
	m_viewMatrix = glm::lookAt(posRelToLookAt + m_lookAt, m_lookAt, glm::vec3(0.0f, Math::sign(cos(m_angleUp)), 0.0f));
	m_transformMatrix = glm::inverse(m_viewMatrix);
}

glm::vec3 Camera::xAxis() const { return glm::normalize(glm::column(m_transformMatrix, 0)); }
glm::vec3 Camera::yAxis() const { return glm::normalize(glm::column(m_transformMatrix, 1)); }
glm::vec3 Camera::zAxis() const { return glm::normalize(glm::column(m_transformMatrix, 2)); }
glm::vec3 Camera::position() const { return glm::column(m_transformMatrix, 3); }