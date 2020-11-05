#pragma once

#include "Framework/Renderer.h"
#include "Framework/Exporter.h"
#include "OpenGL/Shader.h"
#include "Camera/Camera.h"
#include "Utility/FileWatcher.h"

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
	void render();

private:
	Shader m_shader;
	FileWatcher m_shaderWatcher;
	Camera m_camera;
	Renderer m_renderer;
	Exporter m_exporter;
	glm::vec3 m_bgColor = glm::vec3(0.478f, 0.674f, 0.792f);
#ifndef NDEBUG
	bool m_bShow_Debug = true;
	bool m_bShow_ImGuiDemo = false;
#endif
};