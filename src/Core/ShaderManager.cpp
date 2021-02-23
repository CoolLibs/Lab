#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/App/RenderState.h>
#include <Cool/Time/Time.h>

ShaderManager::ShaderManager()
	: m_shaderWatcher([this](const char* path) {
			m_shader.createProgram("Cool/Renderer_Fullscreen/fullscreen.vert", path);
	  })
{
	setShaderPath("myShaders/c3ga RayTracing.frag");
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
}

void ShaderManager::setShaderPath(std::string_view path) {
	m_shaderWatcher.setPath(path);
}

void ShaderManager::ImGui() {
	std::string path = m_shaderWatcher.path().string();
	if (ImGui::InputText("path", &path)) {
		setShaderPath(path);
	}
	if (ImGui::Button("Hi"))
		setShaderPath("");
}