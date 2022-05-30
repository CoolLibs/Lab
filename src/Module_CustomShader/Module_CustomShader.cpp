#include "Module_CustomShader.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/Log/ToUser.h>
#include <ranges>
#include <sstream>

namespace Lab {

Module_CustomShader::Module_CustomShader(DirtyFlagFactory dirty_flag_factory)
    : Module{dirty_flag_factory}
    , _shader_is_dirty{dirty_flag_factory.create()}
    , _file{_shader_is_dirty}
    , _camera_slot{dirty_flag()}
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
    assert(false); // This isn't used at the moment because we set the camera3d manually for all shaders, but this should be changed
    // Cool::CameraShaderU::set_uniform(shader, value);
}

void Module_CustomShader::render(RenderParams in)
{
    refresh_pipeline_if_necessary(in.provider, in.dirty_manager, in.input_slot_destructor);
    Cool::Log::ToUser::info("Custom Shader Render", "Re-rendering");
    if (_fullscreen_pipeline.shader()) {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_time", in.provider(InputSlot_Time{}));

        for (auto& dep : _parameters) {
            std::visit([&](auto&& dep) {
                set_uniform(*_fullscreen_pipeline.shader(), dep.name(), in.provider(dep));
            },
                       dep);
        }
        Cool::CameraShaderU::set_uniform(*_fullscreen_pipeline.shader(), in.provider(_camera_slot), in.provider(InputSlot_AspectRatio{}));
        _fullscreen_pipeline.draw();
    }
    in.dirty_manager.set_clean(dirty_flag());
}

void Module_CustomShader::refresh_pipeline_if_necessary(InputProvider provider, DirtyManager dirty_manager, InputSlotDestructorRef input_slot_destructor)
{
    if (dirty_manager.is_dirty(_shader_is_dirty)) {
        Cool::Log::ToUser::info("Custom Shader Pipeline", "Re-building pipeline");
        const auto file_path   = provider(_file);
        const auto source_code = Cool::File::to_string(file_path.string());
        compile_shader(source_code, file_path.string());
        parse_shader_for_params(source_code, input_slot_destructor);
        dirty_manager.set_clean(_shader_is_dirty);
    }
}

void Module_CustomShader::compile_shader(std::string_view fragment_shader_source_code, std::string_view shader_name)
{
    _fullscreen_pipeline.compile(fragment_shader_source_code, shader_name);
}

static auto name(const AnyInputSlot& slot)
{
    return std::visit(([](auto&& slot) { return slot.name(); }), slot);
}

static auto input_slots_have_the_same_type_and_name(const AnyInputSlot& slot1, const AnyInputSlot& slot2) -> bool
{
    return slot1.index() == slot2.index() &&
           name(slot1) == name(slot2);
}

static auto iterator_to_same_slot(const AnyInputSlot& slot, std::vector<AnyInputSlot>& old_slots)
{
    return std::ranges::find_if(old_slots, [&](const AnyInputSlot& other_slot) {
        return input_slots_have_the_same_type_and_name(other_slot, slot);
    });
}

static void keep_values_of_slots_that_already_existed_and_destroy_unused_ones(
    std::vector<AnyInputSlot>& old_slots,
    std::vector<AnyInputSlot>& new_slots,
    InputSlotDestructorRef     destroy)
{
    for (auto& slot : old_slots) {
        const auto it = iterator_to_same_slot(slot, new_slots);
        if (it != new_slots.end()) {
            *it = std::move(slot);
        }
        else {
            destroy(slot);
        }
    }
}

static auto get_slots_from_shader_code(std::string_view source_code, DirtyFlag dirty_flag)
    -> std::vector<AnyInputSlot>
{
    std::vector<AnyInputSlot> new_params;
    std::stringstream         stream{std::string{source_code}};
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
                            return InputSlot<int>{dirty_flag, name};
                        else if (type == "float")
                            return InputSlot<float>{dirty_flag, name};
                        else if (type == "vec2")
                            return InputSlot<glm::vec2>{dirty_flag, name};
                        else if (type == "vec3")
                            return InputSlot<glm::vec3>{dirty_flag, name};
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
    return new_params;
}

void Module_CustomShader::parse_shader_for_params(std::string_view source_code, InputSlotDestructorRef input_slot_destructor)
{
    auto new_slots = get_slots_from_shader_code(source_code, dirty_flag());
    keep_values_of_slots_that_already_existed_and_destroy_unused_ones(_parameters, new_slots, input_slot_destructor);
    _parameters = std::move(new_slots);
}

} // namespace Lab
