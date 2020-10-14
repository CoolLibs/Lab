#pragma once

class Input {
public:
	static void Initialize();

	static bool KeyIsDown(SDL_Scancode key);
	// Relative to the viewing area, not the whole window !
	static glm::ivec2 MouseInPixels();
	// Relative to the viewing area, not the whole window !
	static glm::vec2 MouseInCentimeters();
	// Relative to the viewing area, not the whole window !
	static glm::vec2  MouseInNormalizedRatioSpace();

private:
	static glm::vec2 s_DPCM; // Dots per Centimeter ; like DPI but in a sensible unit
};