#include "ShaderManager_FromText.h"
#include <Cool/Camera/Camera.h>
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include <fstream>

ShaderManager_FromText::ShaderManager_FromText()
    : _file_watcher{Cool::File::root_dir() + "/shader-examples/axes.frag",
                    /* on_file_changed: */ [&](std::string_view path) { compile_shader(path); },
                    /* on_path_invalid: */ [&](std::string_view path) { 
                        Cool::Log::ToUser::error("Live Coding", "Invalid path: \"{}\"", path); 
                        _fullscreen_pipeline.reset(); }}
{
}

void ShaderManager_FromText::compile_shader(std::string_view path)
{
    _fullscreen_pipeline.compile(Cool::File::to_string(path), path);
    parse_shader_for_params(path);
}

void ShaderManager_FromText::parse_shader_for_params(std::string_view path)
{
    Cool::ParameterDynamicList new_params;
    std::ifstream              stream(path);
    std::string                line;
    bool                       has_begun = false;
    while (getline(stream, line)) {
        if (has_begun) {
            if (!line.compare("// END DYNAMIC PARAMS")) {
                break;
            }
            try {
                const auto uniform_pos = line.find("uniform");
                if (uniform_pos != std::string::npos) {
                    const auto        type_pos     = uniform_pos + 8;
                    const auto        type_pos_end = line.find(' ', type_pos);
                    const std::string type         = line.substr(type_pos, type_pos_end - type_pos);
                    const auto        name_pos     = type_pos_end + 1;
                    const auto        name_pos_end = line.find_first_of(" ;", name_pos);
                    const std::string name         = line.substr(name_pos, name_pos_end - name_pos);
                    //
                    const size_t param_idx = _parameters.index_of(name);
                    if (param_idx != -1) {
                        new_params->push_back((*_parameters)[param_idx]);
                    }
                    else {
                        if (!type.compare("int"))
                            new_params->push_back(Cool::Parameter::Int(name));
                        else if (!type.compare("float"))
                            new_params->push_back(Cool::Parameter::Float(name));
                        else if (!type.compare("vec2"))
                            new_params->push_back(Cool::Parameter::Vec2(name));
                        else if (!type.compare("vec3"))
                            new_params->push_back(Cool::Parameter::Color(name));
                    }
                }
            }
            catch (const std::exception& e) {
                Cool::Log::ToUser::error("ShaderManager_FromText::parse_shader_for_params", "Error while parsing :\n{}", e.what());
            }
        }
        if (!line.compare("// BEGIN DYNAMIC PARAMS")) {
            has_begun = true;
        }
    }
    *_parameters = std::move(*new_params);
}

void ShaderManager_FromText::setup_for_rendering(const Cool::Camera& camera, float time)
{
    if (_fullscreen_pipeline.shader().has_value()) {
        ShaderManager::setup_for_rendering(camera, time);
        _parameters.set_uniforms_in_shader(*_fullscreen_pipeline.shader());
    }
}

void ShaderManager_FromText::set_shader_path(std::string_view path)
{
    _file_watcher.set_path(path);
}

void ShaderManager_FromText::imgui_window()
{
    ImGui::Begin("Shader");
    std::string path = _file_watcher.path().string();
    ImGui::Text("Path : ");
    ImGui::SameLine();
    ImGui::PushID(2132541);
    if (ImGui::InputText("", &path)) {
        set_shader_path(path);
    }
    ImGui::PopID();
    ImGui::SameLine();
    if (Cool::ImGuiExtras::open_file_dialog(&path, Cool::NfdFileFilter::FragmentShader, Cool::File::whithout_file_name(path))) {
        set_shader_path(path);
    }
    ImGui::Separator();
    ImGui::NewLine();
    _parameters.imgui();
    ImGui::End();
}