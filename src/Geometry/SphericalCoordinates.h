#pragma once

#include "SphericalCoordinates_AngularPart.h"

struct SphericalCoordinates {
	SphericalCoordinates(float radius = 1.0f, float angleGround = 0.0f, float angleUp = 0.0f)
		: m_radius(radius), m_angles(angleGround, angleUp), m_xyz(0.0f),
		  m_bMustRecomputeXYZ(true)
	{}

	bool _ImGui_CoordinatesSliders();

	inline float& radius()      { m_bMustRecomputeXYZ = true;  return m_radius; }
	inline float& angleGround() { m_bMustRecomputeXYZ = true;  return m_angles.angleGround(); }
	inline float& angleUp()     { m_bMustRecomputeXYZ = true;  return m_angles.angleUp(); }

	inline const float getRadius()      const { return m_radius; }
	inline const float getAngleGround() const { return m_angles.getAngleGround(); }
	inline const float getAngleUp()     const { return m_angles.getAngleUp(); }

	inline const glm::vec3 getXYZ() { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz; }
	inline const float getX()       { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz.x; }
	inline const float getY()       { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz.y; }
	inline const float getZ()       { if (m_bMustRecomputeXYZ) computeXYZ(); return m_xyz.z; }

private:
	void computeXYZ();

private:
	float m_radius;
	SphericalCoordinates_AngularPart m_angles;

	glm::vec3 m_xyz;
	bool m_bMustRecomputeXYZ;
};