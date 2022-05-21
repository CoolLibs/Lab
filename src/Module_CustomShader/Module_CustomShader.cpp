#include "Module_CustomShader.h"
#include <Cool/Log/ToUser.h>

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

void Module_CustomShader::render(InputProvider provider, DirtyManager dirty_manager)
{
    refresh_pipeline_if_necessary(provider, dirty_manager);
    Cool::Log::ToUser::info("Custom Shader Render", "Re-rendering");
    if (_fullscreen_pipeline.shader()) {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_time", provider(InputSlot_Time{}));
        // Cool::CameraShaderU::set_uniform(*_fullscreen_pipeline.shader(), camera);
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
    // Cool::ParameterList new_params;
    // std::ifstream       stream{std::string{path}};
    // std::string         line;
    // bool                has_begun = false;
    // while (getline(stream, line)) {
    //     if (has_begun) {
    //         if (!line.compare("// END DYNAMIC PARAMS")) {
    //             break;
    //         }
    //         try {
    //             const auto uniform_pos = line.find("uniform");
    //             if (uniform_pos != std::string::npos) {
    //                 const auto        type_pos     = uniform_pos + 8;
    //                 const auto        type_pos_end = line.find(' ', type_pos);
    //                 const std::string type         = line.substr(type_pos, type_pos_end - type_pos);
    //                 const auto        name_pos     = type_pos_end + 1;
    //                 const auto        name_pos_end = line.find_first_of(" ;", name_pos);
    //                 const std::string name         = line.substr(name_pos, name_pos_end - name_pos);
    //                 //
    //                 const auto desc = [&]() -> Cool::Parameter::AnyDesc {
    //                     if (type == "int")
    //                         return Cool::Parameter::IntDesc{.name = name};
    //                     else if (type == "float")
    //                         return Cool::Parameter::FloatDesc{.name = name};
    //                     else if (type == "vec2")
    //                         return Cool::Parameter::Vec2Desc{.name = name};
    //                     else if (type == "vec3")
    //                         return Cool::Parameter::ColorDesc{.name = name};
    //                     else
    //                         throw std::invalid_argument(type + " is not a valid parameter type.");
    //                 }();
    //                 new_params->push_back(Cool::ParameterU::make_param(_parameters, desc));
    //             }
    //         }
    //         catch (const std::exception& e) {
    //             Cool::Log::ToUser::error("ShaderManager_FromText::parse_shader_for_params", "Error while parsing :\n{}", e.what());
    //         }
    //     }
    //     if (!line.compare("// BEGIN DYNAMIC PARAMS")) {
    //         has_begun = true;
    //     }
    // }
    // *_parameters = std::move(*new_params);
}

} // namespace Lab
