#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/App/RenderState.h>
#include <Cool/Time/Time.h>
#include <Cool/Params/Params.h>
#include <Cool/File/File.h>

ShaderManager::ShaderManager()
	: m_shaderWatcher([this](const char* path) { compile_shader(path); })
{
	setShaderPath("shader-examples/simple3D.frag");
}

void ShaderManager::compile_shader(const char* path) {
	m_shader.createProgram("Cool/Renderer_Fullscreen/fullscreen.vert", path);
	parse_shader_for_params(path);
}

void ShaderManager::parse_shader_for_params(const char* path) {
	_dynamic_params.clear();
	std::ifstream stream(path);
	std::string line;
	bool began = false;
	while (getline(stream, line)) {
		if (began) {
			if (!line.compare("// END DYNAMIC PARAMS"))
				break;
			try {
				const auto uniform_pos = line.find("uniform");
				if (uniform_pos != std::string::npos) {
					const auto type_pos = uniform_pos + 8;
					const auto type_pos_end = line.find(' ', type_pos);
					const std::string type = line.substr(type_pos, type_pos_end - type_pos);
					const auto name_pos = type_pos_end + 1;
					const auto name_pos_end = line.find_first_of(" ;", name_pos);
					const std::string name = line.substr(name_pos, name_pos_end - name_pos);
					if (!type.compare("int"))
						_dynamic_params.push_back(std::make_unique<Param::Int>(name));
					else if (!type.compare("float"))
						_dynamic_params.push_back(std::make_unique<Param::Float>(name));
					else if (!type.compare("vec2"))
						_dynamic_params.push_back(std::make_unique<Param::Vec2>(name));
					else if (!type.compare("vec3"))
						_dynamic_params.push_back(std::make_unique<Param::Color>(name));
				}
			}
			catch (std::exception e) {
				Log::Warn(e.what());
			}
		}
		if (!line.compare("// BEGIN DYNAMIC PARAMS"))
			began = true;
	}
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