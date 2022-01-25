#include "Is0.h"
#include <Cool/Input/Input.h>
#include "CodeGen.h"
#include "NodeEditorSerialization.h"

void Is0::update()
{
    if (_editor.tree_has_changed() || _must_recompile) {
        _must_recompile = false;
        if (_editor.tree_is_valid()) {
            _shader_code = CodeGen::full_shader_code(_editor.tree(), _editor.node_templates(), _effects.render_effects);
        }
        else {
            _shader_code = "void main() { gl_FragColor = vec4(vec3(0.), 1.); }";
        }
        _fullscreen_pipeline.compile(_shader_code, "is0 Ray Marcher");
    }
}

void Is0::imgui_windows()
{
    _editor.imgui_window();
    _shader_code_window.show([&]() {
        if (ImGui::InputTextMultiline("##is0 shader code", &_shader_code, ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35))) {
            _fullscreen_pipeline.compile(_shader_code, "is0 Ray Marcher");
        }
    });
    ImGui::Begin("is0 Opt");
    _shader_code_window.open_close_checkbox();
    if (ImGui::Button("Refresh node templates and effects")) {
        _editor.update_templates_and_nodes();
        _effects.render_effects = reload_effects(_effects.render_effects_folder_path, _effects.render_effects);
    }
    // To save a file
    const std::vector<nfdfilteritem_t> is0geometry_file_filter = {
        {"Save", "is0geometry"},
    };
    pick_file_path_to_save(_folder_path_for_save, _file_name_for_save);
    const std::string saving_path = saving_path_string();
    if (Cool::File::exists(saving_path)) {
        Cool::ImGuiExtras::warning_text("This file already exists. Are you sure you want to overwrite it?");
    }
    if (ImGui::Button("Save")) {
        Cool::Serialization::to_json(_editor, saving_path);
    }
    // To load a file
    ImGui::Text("Load a save");
    ImGui::SameLine();
    ImGui::PushID(485);
    std::string _path_for_load;
    if (Cool::ImGuiExtras::open_file_dialog(&_path_for_load, is0geometry_file_filter, _folder_path_for_save)) {
        Cool::Serialization::from_json(_editor, _path_for_load);
    }
    ImGui::PopID();

    ImGui::End();
    _must_recompile |= effect_imgui_window(_effects.render_effects);
}

void Is0::on_key_pressed(const Cool::KeyboardEvent& event)
{
    if (event.action == GLFW_PRESS && Cool::Input::matches_char("a", event.key)) {
        _editor.open_menu();
    }
}

std::string Is0::saving_path_string() const
{
    return std::filesystem::weakly_canonical(_folder_path_for_save + "/" + _file_name_for_save + ".is0geometry").string();
}