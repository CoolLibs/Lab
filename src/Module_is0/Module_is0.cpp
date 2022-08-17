#include "Module_is0.h"
#include <Cool/Camera/CameraShaderU.h>
#include <Cool/Input/Input.h>
#include "CodeGen.h"
#include "Common/make_shader_compilation_error_message.h"
#include "NodeEditorSerialization.h"

namespace Lab {

Module_is0::Module_is0(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"is0", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(dirty_flag(), "Camera")}
{
}

void Module_is0::update(UpdateContext_Ref update_ctx)
{
    if (_editor.tree_has_changed() || _must_regenerate_shader_code)
    {
        _must_regenerate_shader_code = false;
        _must_recompile              = true;
    }
    if (_must_recompile)
    {
        recompile(update_ctx);
    }
}

void Module_is0::remove_all_nodes()
{
    _editor.remove_all_nodes();
    _must_recompile              = true;
    _must_regenerate_shader_code = true;
}

void Module_is0::recompile(UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    if (_editor.tree_is_valid())
    {
        _shader_code = CodeGen::full_shader_code(_editor.tree(), _editor.node_templates(), _effects.render_effects);
    }
    else
    {
        _shader_code = "void main() { gl_FragColor = vec4(vec3(0.), 1.); }";
    }
    _must_recompile      = false;
    const auto maybe_err = _shader.compile(
        _shader_code,
        update_ctx
    );
    if (!for_testing_nodes)
    {
        maybe_err.send_error_if_any(_shader_compilation_error_id, [&](const std::string& msg) {
            return make_shader_compilation_error_message(name(), "Ray Marcher", msg);
        });
    }
    else
    {
        maybe_err.send_error_if_any(
            [&](const std::string& msg) {
                return make_shader_compilation_error_message("Test is0 Nodes", "Ray Marcher", msg);
            },
            Cool::Log::Debug::console()
        );
    }
}

void Module_is0::imgui_windows(Ui_Ref) const
{
    // TODO(JF) Use `ui`

    _shader_code_window.show([&]() {
        if (ImGui::InputTextMultiline("##is0 shader code", &_shader_code, ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35)))
        {
            _must_recompile = true;
            // ui.set_dirty()
            // _shader.compile(_shader_code, "is0 Ray Marcher", ); // TODO(JF) just set shader dirty
        }
    });
    ImGui::Begin("is0 Opt");
    _shader_code_window.open_close_checkbox();
    if (ImGui::Button("Refresh node templates and effects"))
    {
        _editor.update_templates_and_nodes();
        _effects.render_effects = reload_effects(_effects.render_effects_folder_path, _effects.render_effects);
    }
    // To save a file
    const std::vector<nfdfilteritem_t> is0geometry_file_filter = {
        {"Save", "is0geometry"},
    };
    pick_file_path_to_save(_folder_path_for_save, _file_name_for_save);
    const std::string saving_path = saving_path_string();
    if (Cool::File::exists(saving_path))
    {
        Cool::ImGuiExtras::warning_text("This file already exists. Are you sure you want to overwrite it?");
    }
    if (ImGui::Button("Save"))
    {
        Cool::Serialization::to_json(_editor, saving_path);
    }
    // To load a file
    ImGui::Text("Load a save");
    ImGui::SameLine();
    ImGui::PushID(485);
    std::string _path_for_load;
    if (Cool::ImGuiExtras::open_file_dialog(&_path_for_load, is0geometry_file_filter, _folder_path_for_save))
    {
        Cool::Serialization::from_json(_editor, _path_for_load);
    }
    ImGui::PopID();
    // Random
    if (ImGui::Button("Generate a random object"))
    {
        _editor.generate_a_random_scene();
    }

    ImGui::End();
    _must_regenerate_shader_code |= effect_imgui_window(_effects.render_effects);

    // This line has to be at the end to have is0 focused when we open the app
    _editor.imgui_window();
}

std::string Module_is0::saving_path_string() const
{
    return std::filesystem::weakly_canonical(_folder_path_for_save + "/" + _file_name_for_save + ".is0geometry").string();
}

auto Module_is0::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;
    inputs.push_back(Cool::AnyInputRefToConst{_camera_input});
    // for (const auto& input : _inputs) {
    //     inputs.push_back(
    //         std::visit([](auto&& input) { return AnyInputRefToConst{input}; }, input));
    // }
    return inputs;
}

auto Module_is0::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty) ||
           check_dirty(_shader.dirty_flag());
};

void Module_is0::render(RenderParams in, UpdateContext_Ref)
{
    if (_shader.pipeline().shader())
    {
        _shader.pipeline().shader()->bind();
        _shader.pipeline().shader()->set_uniform("_time", in.provider(Cool::Input_Time{}));

        // for (auto& input : _inputs) {
        //     std::visit([&](auto&& input) {
        //         set_uniform(*_shader.pipeline().shader(), input.name(), in.provider(input));
        //     },
        //                input);
        // }
        Cool::CameraShaderU::set_uniform(*_shader.pipeline().shader(), in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{}));
        _shader.pipeline().draw();
    }
}

} // namespace Lab