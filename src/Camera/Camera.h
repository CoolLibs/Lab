#pragma once

#include "CameraState.h"

class Camera {
friend class CameraState_Idle;
friend class CameraState_Rotation;
friend class CameraState_Translation; 
public:
	Camera();
	~Camera() = default;

	inline const glm::mat4& transformMatrix() const { return m_transformMatrix; }
	inline const glm::mat4& viewMatrix() const { return m_viewMatrix; }
	glm::vec3 xAxis() const;
	glm::vec3 yAxis() const;
	glm::vec3 zAxis() const;
	glm::vec3 position() const;
	inline float focalLength() const { return m_focalLength; }

	inline void update() { m_state->update(); }
	inline void onWheelDown() { m_state->onWheelDown(); }
	inline void onWheelUp()   { m_state->onWheelUp();   }
	inline void onWheelScroll(float dl) { m_state->onWheelScroll(dl); }

private:
	void onTransformChanged();
	void onProjectionChanged();

	template <typename T, typename ...Args>
	inline void setState(Args&& ...args) {
		m_state = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

private:
	glm::vec3 m_lookAt = glm::vec3(0.0f);
	float m_distToLookAt = 3.0f;
	float m_angleGround = 0.0f;
	float m_angleUp = 0.0f;

	glm::mat4 m_transformMatrix;
	glm::mat4 m_viewMatrix;

	float m_focalLength = 1.0f;

	std::unique_ptr<CameraState> m_state;
};