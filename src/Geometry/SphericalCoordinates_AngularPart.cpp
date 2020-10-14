#include "SphericalCoordinates_AngularPart.h"

void SphericalCoordinates_AngularPart::computeXYZ() {
	m_xyz.x = cos(getAngleUp()) * cos(getAngleGround());
	m_xyz.y = sin(getAngleUp());
	m_xyz.z = cos(getAngleUp()) * sin(getAngleGround());
	m_bMustRecomputeXYZ = false;
}

bool SphericalCoordinates_AngularPart::_ImGui_CoordinatesSliders() {
	return _ImGui_AngleUpSlider() || _ImGui_AngleGroundSlider();
}

bool SphericalCoordinates_AngularPart::_ImGui_AngleUpSlider() {
	bool modifs = ImGui::SliderAngle("Angle Up", &angleUp(), -180, 0);
	if (modifs)
		m_bMustRecomputeXYZ = true;
	return modifs;
}

bool SphericalCoordinates_AngularPart::_ImGui_AngleGroundSlider() {
	bool modifs = ImGui::SliderAngle("Angle Ground", &angleGround());
	if (modifs)
		m_bMustRecomputeXYZ = true;
	return modifs;
}