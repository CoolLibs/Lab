#include "Input.h"

#include "Framework/Viewports.h"

bool Input::KeyIsDown(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[key];
}

glm::vec2 Input::GetMouseInPixels() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::vec2(x, y);
}

glm::vec2 Input::GetMouseInNormalizedRatioSpace() {
	glm::vec2 pos = GetMouseInPixels();
	pos -= Viewports::AppView().topLeft();
	pos /= Viewports::AppView().height();
	pos.y = 1.0f - pos.y;
	pos *= 2.0f;
	pos -= glm::vec2(Viewports::AppView().aspectRatio(), 1.0f);
	return pos;
}

bool Input::IsMouseButtonDown(int button) {
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}