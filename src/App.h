#pragma once

#include <Cool/App/IApp.h>
#include <Cool/Renderer_Fullscreen/Renderer_Fullscreen.h>
#include "Framework/Exporter.h"
#include <Cool/OpenGL/Shader.h>
#include <Cool/Camera/Camera.h>
#include <Cool/FileWatcher/FileWatcher.h>

using namespace Cool;

class App : public IApp {
public:
	App();
	~App() = default;

	void update() override;
	void ImGuiWindows() override;
	void ImGuiMenus() override;
	void onEvent(const SDL_Event& e) override;

private:
	void render();

private:
	Shader m_shader;
	FileWatcher m_shaderWatcher;
	Camera m_camera;
	Renderer_Fullscreen m_renderer;
	Exporter m_exporter;
	glm::vec3 m_bgColor = glm::vec3(0.478f, 0.674f, 0.792f);
#ifndef NDEBUG
	bool m_bShow_Debug = true;
	bool m_bShow_ImGuiDemo = false;
#endif
};