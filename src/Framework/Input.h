#pragma once

class Input {
public:
	static bool KeyIsDown(SDL_Scancode key);
	/// Relative to the viewing area, not the whole window !
	/// (0, 0) is in the top-left corner
	static glm::ivec2 MouseInPixels();
	/// Relative to the viewing area, not the whole window !
	/// (0, 0) is in the top-left corner
	static glm::vec2 MouseInCentimeters();
	/// Relative to the viewing area, not the whole window !
	/// (0, 0) is in the middle
	/// x is in the range [-aspectRatio, +aspectRatio] Axis pointing right
	/// y is in the range [-1, 1] Axis poiting up
	static glm::vec2  MouseInNormalizedRatioSpace();

private:
friend class AppFramework;
	static void Initialize();

private:
	/// Dots per Centimeter ; like DPI but in a sensible unit
	static glm::vec2 s_DPCM;
#ifndef NDEBUG
	static bool s_initialized;
#endif
};