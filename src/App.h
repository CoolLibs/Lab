#pragma once

#include "Framework/Renderer.h"
#include "OpenGL/Shader.h"
#include "Camera/Camera.h"

class App {
public:
	App();
	~App() = default;

	void update();
	void ImGuiWindows();
	void ImGuiMenus();
	void onEvent(const SDL_Event& e);
	void onRenderAreaResized();

private:
	float m_focalLength = 2.7f;
	Camera m_camera;
	Renderer m_renderer;
	Shader m_shader;
	glm::vec3 m_bgColor = glm::vec3(0.478f, 0.674f, 0.792f);
#ifndef NDEBUG
	bool m_bShow_Debug = true;
	bool m_bShow_ImGuiDemo = false;
#endif
};