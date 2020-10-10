#pragma once

struct GLWindow {
	GLWindow(SDL_Window* _window)
		: window(_window) 
	{
		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			Log::Error("Failed to create OpenGL context : {}", SDL_GetError());
		}
	}
	inline void destroy() { SDL_DestroyWindow(window); }

	SDL_Window* window;
	SDL_GLContext glContext;

	inline void makeCurrent() { SDL_GL_MakeCurrent(window, glContext); }
	bool checkForFullscreenToggles(const SDL_Event& e);
	void switchFullScreen();
	void escapeFullScreen();

private:
	bool m_bIsFullScreen = false;
};