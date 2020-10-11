#include "Input.h"

#include "Framework/RenderState.h"

bool Input::KeyIsDown(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[key];
}

glm::ivec2 Input::MouseInPixels() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::ivec2(x, y) - RenderState::AppView().topLeft();
}

glm::vec2 Input::MouseInNormalizedRatioSpace() {
	glm::vec2 pos = MouseInPixels();
	pos /= RenderState::AppView().height();
	pos.y = 1.0f - pos.y;
	pos *= 2.0f;
	pos -= glm::vec2(RenderState::AppView().aspectRatio(), 1.0f);
	return pos;
}