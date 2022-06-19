#include "ShaderManager_FromText.h"
#include <Cool/Camera/Camera.h>
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include <Cool/Parameter/ParameterU.h>
#include <Cool/Parameter/set_uniform.h>
#include <Cool/Path/Path.h>
#include <fstream>
#include <glpp/glpp.hpp>

ShaderManager_FromText::ShaderManager_FromText()
    : _file_watcher{Cool::Path::root() + "/shader-examples/axes.frag",
                    {.on_file_changed = [&](std::string_view path) { compile_shader(path); },
                     .on_path_invalid = [&](std::string_view path) { 
                        Cool::Log::ToUser::error("Live Coding", "Invalid path: \"{}\"", path); 
                        _fullscreen_pipeline.reset(); }}}
{
}

void ShaderManager_FromText::compile_shader(std::string_view path)
{
    _fullscreen_pipeline.compile(Cool::File::to_string(path), path);
    parse_shader_for_params(path);
}

void ShaderManager_FromText::parse_shader_for_params(std::string_view path)
{
    Cool::ParameterList new_params;
    std::ifstream       stream{std::string{path}};
    std::string         line;
    bool                has_begun = false;
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
                    const auto desc = [&]() -> Cool::Parameter::AnyDesc {
                        if (type == "int")
                            return Cool::Parameter::IntDesc{.name = name};
                        else if (type == "float")
                            return Cool::Parameter::FloatDesc{.name = name};
                        else if (type == "vec2")
                            return Cool::Parameter::Vec2Desc{.name = name};
                        else if (type == "vec3")
                            return Cool::Parameter::ColorDesc{.name = name};
                        else
                            throw std::invalid_argument(type + " is not a valid parameter type.");
                    }();
                    new_params->push_back(Cool::ParameterU::make_param(_parameters, desc));
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

void ShaderManager_FromText::setup_for_rendering(const Cool::Camera& camera, float time, float aspect_ratio)
{
    if (_fullscreen_pipeline.shader().has_value()) {
        ShaderManager::setup_for_rendering(camera, time, aspect_ratio);
        set_uniforms(*_fullscreen_pipeline.shader(), _parameters);
    }
}

void ShaderManager_FromText::set_image_in_shader(std::string_view name, int slot, GLuint texture_id)
{
    if (_fullscreen_pipeline.shader().has_value()) {
        _fullscreen_pipeline.shader()->bind();
        glpp::active_texture(slot);
        glpp::bind_texture<glpp::TextureKind::Tex2D>(texture_id);
        _fullscreen_pipeline.shader()->set_uniform(name, slot);
        set_uniforms(*_fullscreen_pipeline.shader(), _parameters);
    }
}

void ShaderManager_FromText::set_shader_path(std::string_view path)
{
    _file_watcher.set_path(path);
}

void ShaderManager_FromText::imgui_windows()
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