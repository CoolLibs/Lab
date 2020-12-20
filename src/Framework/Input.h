#pragma once

class Input {
public:
	static bool KeyIsDown(SDL_Scancode key);
	// Relative to the viewing area, not the whole window !
	static glm::ivec2 MouseInPixels();
	// Relative to the viewing area, not the whole window !
	static glm::vec2 MouseInCentimeters();
	// Relative to the viewing area, not the whole window !
	static glm::vec2  MouseInNormalizedRatioSpace();

private:
friend class AppFramework;
	static void Initialize();

private:
	// Dots per Centimeter ; like DPI but in a sensible unit
	static glm::vec2 s_DPCM;
};