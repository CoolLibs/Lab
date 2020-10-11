#pragma once

class Input {
public:
	static bool KeyIsDown(SDL_Scancode key);
	// Relative to the viewing area, not the whole window !
	static glm::ivec2 MouseInPixels();
	// Relative to the viewing area, not the whole window !
	static glm::vec2  MouseInNormalizedRatioSpace();
};