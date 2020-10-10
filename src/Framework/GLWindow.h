#pragma once

struct GLWindow {
	GLWindow(SDL_Window* _window)
		: window(_window) 
	{
		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr) {
			spdlog::critical("[SDL2] OpenGL context is null: {}", SDL_GetError());
			debug_break();
		}
	}
	GLWindow(SDL_Window* _window, SDL_GLContext _glContext)
		: window(_window), glContext(_glContext)
	{}
	inline void destroy() { SDL_DestroyWindow(window); }

	SDL_Window* window;
	SDL_GLContext glContext;

	inline void makeCurrent() { SDL_GL_MakeCurrent(window, glContext); }
	inline void hide() { SDL_HideWindow(window); }
	inline void show() { SDL_ShowWindow(window); }
	bool checkForFullscreenToggles(const SDL_Event& e);
	void switchFullScreen();
	void escapeFullScreen();

private:
	bool m_bIsFullScreen = false;
};