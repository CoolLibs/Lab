#pragma once

#include "CameraState.h"
#include "Geometry/SphericalCoordinates.h"
#include "Geometry/Ray.h"
#include <glm/gtc/matrix_access.hpp>

class Camera {
friend class CameraState_Rest;
friend class CameraState_Rotation;
friend class CameraState_Translation; 
public:
	Camera();
	~Camera() = default;

	inline const glm::mat4& transformMatrix() const { return m_transformMatrix; }

	inline void update() { m_state->update(); }
	inline void onWheelDown() { m_state->onWheelDown(); }
	inline void onWheelUp()   { m_state->onWheelUp();   }
	inline void onWheelScroll(float dl) { m_state->onWheelScroll(dl); }
	inline void onKeyPressed(SDL_Scancode scancode) { m_state->onKeyPressed(scancode); }
	inline glm::vec3 xAxis() const { return glm::normalize(glm::column(transformMatrix(), 0)); }
	inline glm::vec3 yAxis() const { return glm::normalize(glm::column(transformMatrix(), 1)); }
	inline glm::vec3 zAxis() const { return glm::normalize(glm::column(transformMatrix(), 2)); }
	inline const glm::vec3& position() const { return glm::column(transformMatrix(), 3); }
	inline       glm::vec3  position() { return glm::column(transformMatrix(), 3); }

private:
	inline const float& angleGround() const { return m_angleGround; }
	inline       float& angleGround()       { return m_angleGround; }
	inline const float& angleUp() const { return m_angleUp; }
	inline       float& angleUp()       { return m_angleUp; }
	void onTransformChanged();
	void onProjectionChanged();

	template <typename T, typename ...Args>
	inline void setState(Args&& ...args) {
		m_state = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

public:
	float m_focalLength = 1.0f;

	glm::vec3 m_lookAt = glm::vec3(0.0f);
	float m_distToLookAt = 3.0f;
	float m_angleGround = 0.0f;
	float m_angleUp = 0.0f;

	glm::mat4 m_transformMatrix;
	glm::mat4 m_inverseTransformMatrix;

	std::unique_ptr<CameraState> m_state;
};