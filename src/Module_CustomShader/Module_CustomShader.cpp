#include "Module_CustomShader.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/Log/ToUser.h>
#include <sstream>

namespace Lab {

Module_CustomShader::Module_CustomShader(DirtyFlagFactory dirty_flag_factory)
    : _shader_is_dirty{dirty_flag_factory.create()}
    , _file{_shader_is_dirty}
{
}

void Module_CustomShader::imgui_windows(Ui ui)
{
    Ui::window({.name = "Custom Shader"}, [&]() {
        ui.widget("File", _file);
        ImGui::Separator();
        ImGui::NewLine();
        for (auto& dep : _parameters) {
            std::visit([&ui](auto&& dep) {
                ui.widget(dep.name(), dep);
            },
                       dep);
        }
    });
}

template<typename T>
void set_uniform(const Cool::OpenGL::Shader& shader, std::string_view name, const T& value)
{
    shader.set_uniform(name, value);
}

void set_uniform(const Cool::OpenGL::Shader& shader, std::string_view name, const Cool::Camera& value)
{
    Cool::CameraShaderU::set_uniform(shader, value);
}

void Module_CustomShader::render(InputProvider provider, DirtyManager dirty_manager)
{
    refresh_pipeline_if_necessary(provider, dirty_manager);
    Cool::Log::ToUser::info("Custom Shader Render", "Re-rendering");
    if (_fullscreen_pipeline.shader()) {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_time", provider(InputSlot_Time{}));

        for (auto& dep : _parameters) {
            std::visit([&](auto&& dep) {
                set_uniform(*_fullscreen_pipeline.shader(), dep.name(), provider(dep));
            },
                       dep);
        }
        Cool::CameraShaderU::set_uniform(*_fullscreen_pipeline.shader(), provider(InputSlot_Camera{}));
        _fullscreen_pipeline.draw();
    }
    _is_dirty = false;
}

void Module_CustomShader::refresh_pipeline_if_necessary(InputProvider provider, DirtyManager dirty_manager)
{
    if (dirty_manager.is_dirty(_shader_is_dirty)) {
        Cool::Log::ToUser::info("Custom Shader Pipeline", "Re-building pipeline");
        const auto file_path = provider(_file);
        compile_shader(Cool::File::to_string(file_path.string()),
                       file_path.string());
        dirty_manager.set_clean(_shader_is_dirty);
    }
}

void Module_CustomShader::compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name)
{
    _fullscreen_pipeline.compile(fragment_shader_source_code, shader_name);
    parse_shader_for_params(fragment_shader_source_code);
}

void Module_CustomShader::parse_shader_for_params(std::string_view fragment_shader_source_code)
{
    std::vector<AnyInputSlot> new_params;
    std::stringstream         stream{std::string{fragment_shader_source_code}};
    std::string               line;
    bool                      has_begun = false;
    while (getline(stream, line)) {
        if (has_begun) {
            if (line == "// END DYNAMIC PARAMS") {
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
                    const auto input_slot = [&]() -> AnyInputSlot {
                        if (type == "int")
                            return InputSlot<int>{name};
                        else if (type == "float")
                            return InputSlot<float>{name};
                        else if (type == "vec2")
                            return InputSlot<glm::vec2>{name};
                        else if (type == "vec3")
                            return InputSlot<glm::vec3>{name};
                        else
                            throw std::invalid_argument(type + " is not a valid parameter type.");
                    }();
                    new_params.push_back(input_slot);
                }
            }
            catch (const std::exception& e) {
                Cool::Log::ToUser::error("ShaderManager_FromText::parse_shader_for_params", "Error while parsing :\n{}", e.what());
            }
        }
        if (line == "// BEGIN DYNAMIC PARAMS") {
            has_begun = true;
        }
    }
    _parameters = std::move(new_params);
}

} // namespace Lab
