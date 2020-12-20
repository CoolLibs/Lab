#include "Input.h"

#include "RenderState.h"

glm::vec2 Input::s_DPCM;
static constexpr float INCH_TO_CM = 2.54f;
#ifndef NDEBUG
bool Input::s_initialized = false;
#endif

void Input::Initialize() {
	int exitCode = SDL_GetDisplayDPI(0, nullptr, &s_DPCM[0], &s_DPCM[1]);
	if (exitCode == -1) { // Failure
		s_DPCM = glm::vec2(96.f);
		Log::Release::Warn("Couldn't retrieve your screen's DPI. Going for the default 96 DPI.");
	}
	s_DPCM /= INCH_TO_CM;
#ifndef NDEBUG
	if (s_initialized)
		Log::Warn("[Input::Initialize] You are calling Initialize() twice.");
	s_initialized = true;
#endif
}

bool Input::KeyIsDown(SDL_Scancode key) {
	const Uint8* state = SDL_GetKeyboardState(NULL);
	return state[key];
}

glm::ivec2 Input::MouseInPixels() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return glm::ivec2(x, y) - RenderState::InAppRenderArea().topLeft();
}

glm::vec2 Input::MouseInCentimeters() {
#ifndef NDEBUG
	if (!s_initialized)
		Log::Error("You are using Input::MouseInCentimeters() before the Input class is Initialized. Initialize() happens in the constructor of AppFramework.");
#endif
	return static_cast<glm::vec2>(MouseInPixels()) * s_DPCM;
}

glm::vec2 Input::MouseInNormalizedRatioSpace() {
	glm::vec2 pos = MouseInPixels();
	pos /= RenderState::InAppRenderArea().height();
	pos.y = 1.0f - pos.y;
	pos *= 2.0f;
	pos -= glm::vec2(RenderState::InAppRenderArea().aspectRatio(), 1.0f);
	return pos;
}