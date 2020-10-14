#pragma once

#include <glm/glm.hpp>

struct SphericalCoordinates_AngularPart {
	SphericalCoordinates_AngularPart(float angleGround = 0.0f, float angleUp = 0.0f)
		: m_angleGround(angleGround), m_angleUp(angleUp), m_xyz(0.0f),
		  m_bMustRecomputeXYZ(true)
	{}

	bool _ImGui_CoordinatesSliders();
	bool _ImGui_AngleUpSlider();
	bool _ImGui_AngleGroundSlider();

	inline float& angleGround() { m_bMustRecomputeXYZ = true;  return m_angleGround; }
	inline float& angleUp()     { m_bMustRecomputeXYZ = true;  return m_angleUp; }

	inline const float getAngleGround() const { return m_angleGround; }
	inline const float getAngleUp()     const { return m_angleUp; }

	inline const glm::vec3 getXYZ() { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz; }
	inline const float getX()       { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz.x; }
	inline const float getY()       { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz.y; }
	inline const float getZ()       { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz.z; }

private:
	void computeXYZ();

private:
	float m_angleGround;
	float m_angleUp;

	glm::vec3 m_xyz;
	bool m_bMustRecomputeXYZ;
};