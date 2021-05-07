#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/App/RenderState.h>
#include <Cool/Time/Time.h>
#include <Cool/Params/Params.h>
#include <Cool/File/File.h>
#include <Cool/App/NfdFileFilter.h>

ShaderManager::ShaderManager()
	: m_shaderWatcher([this](const char* path) { compile_shader(path); })
{
	setShaderPath("shader-examples/simple3D.frag");
}

void ShaderManager::compile_shader(const char* path) {
	m_shader.create_program("Cool/Renderer_Fullscreen/fullscreen.vert", path);
	parse_shader_for_params(path);
}

void ShaderManager::parse_shader_for_params(const char* path) {
	std::vector<std::unique_ptr<Cool::Internal::IParam>> new_params;
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
					//
					const size_t param_idx = find_param(name);
					if (param_idx != -1) {
						new_params.push_back(std::move(_dynamic_params[param_idx]));
					}
					else {
						if (!type.compare("int"))
							new_params.push_back(std::make_unique<Param::Int>(name));
						else if (!type.compare("float"))
							new_params.push_back(std::make_unique<Param::Float>(name));
						else if (!type.compare("vec2"))
							new_params.push_back(std::make_unique<Param::Vec2>(name));
						else if (!type.compare("vec3"))
							new_params.push_back(std::make_unique<Param::Color>(name));
					}
				}
			}
			catch (std::exception e) {
				Log::Warn(e.what());
			}
		}
		if (!line.compare("// BEGIN DYNAMIC PARAMS"))
			began = true;
	}
	_dynamic_params = std::move(new_params);
}

size_t ShaderManager::find_param(std::string_view name) {
	for (size_t i = 0; i < _dynamic_params.size(); ++i) {
		if (_dynamic_params[i] && !name.compare(_dynamic_params[i]->name()))
			return i;
	}
	return -1;
}

void ShaderManager::setupForRendering(const Camera& camera) {
	m_shader.bind();
	m_shader.set_uniform("uAspectRatio", RenderState::Size().aspectRatio());
	m_shader.set_uniform("uCamX",        camera.xAxis());
	m_shader.set_uniform("uCamY",        camera.yAxis());
	m_shader.set_uniform("uCamZ",        camera.zAxis());
	m_shader.set_uniform("uCamPos",      camera.position());
	m_shader.set_uniform("uFocalLength", camera.focalLength());
	m_shader.set_uniform("uTime", Time::time());
	for (const auto& param : _dynamic_params) {
		param->set_uniform_in_shader(m_shader);
	}
}

void ShaderManager::setShaderPath(std::string_view path) {
	m_shaderWatcher.setPath(path);
}

void ShaderManager::ImGui() {
	std::string path = m_shaderWatcher.path().string();
	ImGui::Text("Path : "); ImGui::SameLine();
	ImGui::PushID(2132541);
	if (ImGui::InputText("", &path)) {
		setShaderPath(path);
	}
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::OpenFileDialog(&path, NfdFileFilter::FragmentShader, File::WhithoutFileName(path))) {
		setShaderPath(path);
	}
	ImGui::Separator();
	ImGui::NewLine();
	for (auto& param : _dynamic_params) {
		param->ImGui({}, []() {});
	}
}