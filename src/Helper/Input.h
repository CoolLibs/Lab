#pragma once

namespace Input {
	bool KeyIsDown(SDL_Scancode key);
	glm::vec2 GetMouseInPixels();
	glm::vec2 GetMouseInNormalizedRatioSpace();
	bool IsMouseButtonDown(int button);
}