#pragma once

#include "Framework/Renderer.h"
#include "OpenGL/Shader.h"

class App {
public:
	App();
	~App() = default;

	void update();
	void onEvent(const SDL_Event& e);
	void onRenderAreaResized();
	void ImGuiWindows();
	void ImGuiMenus();

private:
	Shader m_shader;
	glm::vec3 m_bgColor = glm::vec3(0.478f, 0.674f, 0.792f);
	Renderer m_renderer;
#ifndef NDEBUG
	bool m_bShow_Debug = true;
	bool m_bShow_ImGuiDemo = false;
#endif
};