#pragma once

#include <Cool/App/IApp.h>
#include <Cool/App/Window.h>
#include <Cool/Renderer_Fullscreen/Renderer_Fullscreen.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Camera/Camera.h>
#include <Cool/Camera/ViewController_Orbital.h>
#include <Cool/Camera/ProjectionController_Perspective.h>
#include "ShaderManager/ShaderManagerManager.h"

using namespace Cool;

class App : public IApp {
public:
	App(Window& main_window);
	~App();

	void update() override;
	void ImGuiWindows() override;
	void ImGuiMenus() override;
	void onKeyboardEvent(int key, int scancode, int action, int mods) override;
	void onMouseButtonEvent(int button, int action, int mods) override;
	void onScrollEvent(double xOffset, double yOffset) override;
	void onMouseMoveEvent(double xPos, double yPos) override;

private:
	void render();

private:
	Window& _main_window;
	ShaderManagerManager _shader_manager;
	Camera _camera;
	ViewController_Orbital _camera_orbital_controller;
	ProjectionController_Perspective _camera_perspective_controller;
	Renderer_Fullscreen _renderer;
	Exporter _exporter;
	glm::vec3 _background_color = glm::vec3(0.478f, 0.674f, 0.792f);
#ifdef DEBUG
	bool _show_imgui_debug = true;
	bool _show_imgui_demo = false;
#endif

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("Shader Manager Manager", _shader_manager)
		);
	}
};