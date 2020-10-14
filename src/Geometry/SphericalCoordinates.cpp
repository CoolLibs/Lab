#include "SphericalCoordinates.h"

void SphericalCoordinates::computeXYZ() {
	m_xyz = getRadius() * m_angles.getXYZ();
	m_bMustRecomputeXYZ = false;
}

bool SphericalCoordinates::_ImGui_CoordinatesSliders() {
	bool modifs = ImGui::SliderAngle("Angle Ground", &angleGround());
	modifs |=     ImGui::SliderAngle("Angle Up", &angleUp());
	modifs |=     ImGui::DragFloat("Distance", &radius(), 1.0f);
	if (m_radius < 0.5f) 
		m_radius = 0.5f;
	if (modifs)
		m_bMustRecomputeXYZ = true;
	return modifs;
}