#pragma once

namespace MyImGui {
	void HelpMarker(const char* text);
	bool AngleWheel(const char* label, float* value_p, float thickness = 2.0f, float radius = 25.0f, int circleNbSegments = 26);
	void TimeFormatedHMS(float timeInSec, float totalDuration = 0.0f);
	void Tooltip(const char* text);
	void ButtonDisabled(const char* label, const char* reasonForDisabling = "Currently disabled");
	bool ButtonWithIcon(GLuint texID, const ImVec4& tintColor = ImVec4(1, 1, 1, 1), const ImVec4& backgroundColor = ImVec4(0, 0, 0, 1));
	void ButtonWithIconDisabled(GLuint texID, const char* reasonForDisabling = "Currently disabled");
	void ImageFramed(GLuint texID, const ImVec2& size, int frameThickness = -1, const ImVec4& frameColor = ImVec4(0, 0, 0, 0), const ImVec4& backgroundColor = ImVec4(0, 0, 0, 1), const ImVec4& tintColor = ImVec4(1, 1, 1, 1));
	bool InputUInt(const char* label, unsigned int* value_p);
}