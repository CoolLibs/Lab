#include "GLWindow.h"

bool GLWindow::checkForFullscreenToggles(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.window.windowID == SDL_GetWindowID(window)) {
		if (e.key.keysym.scancode == SDL_SCANCODE_F11) {
			switchFullScreen();
			return true;
		}
		if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && m_bIsFullScreen) {
			escapeFullScreen();
			return true;
		}
	}
	return false;
}

void GLWindow::switchFullScreen() {
	if (m_bIsFullScreen)
		SDL_SetWindowFullscreen(window, 0);
	else
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	m_bIsFullScreen = !m_bIsFullScreen;
}

void GLWindow::escapeFullScreen() {
	SDL_SetWindowFullscreen(window, 0);
	m_bIsFullScreen = false;
}