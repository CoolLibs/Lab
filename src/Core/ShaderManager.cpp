#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/App/RenderState.h>
#include <Cool/Time/Time.h>
#include <Cool/Params/Params.h>

ShaderManager::ShaderManager()
	: m_shaderWatcher([this](const char* path) { compile_shader(path); })
{
	setShaderPath("shader-examples/simple3D.frag");
}

void ShaderManager::compile_shader(const char* path) {
	m_shader.createProgram("Cool/Renderer_Fullscreen/fullscreen.vert", path);
	_dynamic_params.clear();
	_dynamic_params.push_back(std::make_unique<Param::Color>("_color"));
}

void ShaderManager::setupForRendering(const Camera& camera) {
	m_shader.bind();
	m_shader.setUniform1f("uAspectRatio", RenderState::Size().aspectRatio());
	m_shader.setUniform3f("uCamX",        camera.xAxis());
	m_shader.setUniform3f("uCamY",        camera.yAxis());
	m_shader.setUniform3f("uCamZ",        camera.zAxis());
	m_shader.setUniform3f("uCamPos",      camera.position());
	m_shader.setUniform1f("uFocalLength", camera.focalLength());
	m_shader.setUniform1f("uTime", Time::time());
	for (const auto& param : _dynamic_params) {
		param->set_uniform_in_shader(m_shader);
	}
}

void ShaderManager::setShaderPath(std::string_view path) {
	m_shaderWatcher.setPath(path);
}

void ShaderManager::ImGui() {
	std::string path = m_shaderWatcher.path().string();
	if (ImGui::InputText("path", &path)) {
		setShaderPath(path);
	}
	ImGui::Separator();
	for (auto& param : _dynamic_params) {
		param->ImGui({}, []() {});
	}
}