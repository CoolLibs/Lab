#pragma once

#include "App.h"
#include "GLWindow.h"
struct ImGuiDockNode;

class AppFramework {
public:
	AppFramework(GLWindow& glWindow, App& app);
	~AppFramework() = default;

	void update();
	inline bool shouldClose() const { return m_bShouldClose; }

private:
	void onWindowMove();
	void onWindowResize();
	void updateAvailableAppViewSizeAndPos(ImGuiDockNode* node);
	bool onEvent(const SDL_Event& e); // Returns true iff the event has been handled and must not be forwarded to the app
	void ImGuiDockspace();
	inline void closeApp() { m_bShouldClose = true; }

private:
	bool m_bShowUI = true;

	GLWindow& m_glWindow;
	App& m_app;
	bool m_bShouldClose = false;
};
