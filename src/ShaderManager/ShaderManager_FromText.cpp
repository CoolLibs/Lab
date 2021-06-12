#include "ShaderManager_FromText.h"

#include <Cool/Camera/Camera.h>
#include <Cool/File/File.h>
#include <Cool/App/NfdFileFilter.h>
#include <fstream>

ShaderManager_FromText::ShaderManager_FromText()
	: m_shaderWatcher([this](std::string_view path) { compile_shader(path); })
{
	setShaderPath("shader-examples/simple3D.frag");
}

void ShaderManager_FromText::compile_shader(std::string_view path) {
	_shader.create_program("Cool/res/shaders/fullscreen.vert", path);
	parse_shader_for_params(path);
}

void ShaderManager_FromText::parse_shader_for_params(std::string_view path) {
	ParameterDynamicList new_params;
	std::ifstream stream(path);
	std::string line;
	bool has_begun = false;
	while (getline(stream, line)) {
		if (has_begun) {
			if (!line.compare("// END DYNAMIC PARAMS")) {
				break;
			}
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
					const size_t param_idx = _parameters.find(name);
					if (param_idx != -1) {
						new_params->push_back((*_parameters)[param_idx]);
					}
					else {
						if (!type.compare("int"))
							new_params->push_back(Parameter::Int(name));
						else if (!type.compare("float"))
							new_params->push_back(Parameter::Float(name));
						else if (!type.compare("vec2"))
							new_params->push_back(Parameter::Vec2(name));
						else if (!type.compare("vec3"))
							new_params->push_back(Parameter::Color(name));
					}
				}
			}
			catch (std::exception e) {
				Log::ToUser::warn("[ShaderManager_FromText::parse_shader_for_params] Error while parsing :\n{}", e.what());
			}
		}
		if (!line.compare("// BEGIN DYNAMIC PARAMS")) {
			has_begun = true;
		}
	}
	*_parameters = std::move(*new_params);
}

void ShaderManager_FromText::setup_for_rendering(const Camera& camera, float focal_length) {
	ShaderManager::setup_for_rendering(camera, focal_length);
	_parameters.set_uniforms_in_shader(_shader);
}

void ShaderManager_FromText::setShaderPath(std::string_view path) {
	m_shaderWatcher.set_path(path);
}

void ShaderManager_FromText::ImGui_window() {
	ImGui::Begin("Shader");
	std::string path = m_shaderWatcher.path().string();
	ImGui::Text("Path : "); ImGui::SameLine();
	ImGui::PushID(2132541);
	if (ImGui::InputText("", &path)) {
		setShaderPath(path);
	}
	ImGui::PopID();
	ImGui::SameLine();
	if (ImGui::open_file_dialog(&path, NfdFileFilter::FragmentShader, File::whithout_file_name(path))) {
		setShaderPath(path);
	}
	ImGui::Separator();
	ImGui::NewLine();
	_parameters.imgui();
	ImGui::End();
}