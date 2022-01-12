#include "Is0.h"
#include <Cool/Input/Input.h>
#include "CodeGen.h"

void Is0::update()
{
    if (_editor.tree_has_changed() || _must_recompile) {
        _must_recompile = false;
        if (_editor.tree_is_valid()) {
            _shader_code = CodeGen::full_shader_code(_editor.tree(), _editor.node_templates(), _effects);
        }
        else {
            _shader_code = "void main() { gl_FragColor = vec4(vec3(0.), 1.); }";
        }
        _fullscreen_pipeline.compile(_shader_code, "is0 Ray Marcher");
    }
}

void Is0::imgui_window()
{
    _editor.imgui_window();
    _shader_code_window.show([&]() {
        if (ImGui::InputTextMultiline("##is0 shader code", &_shader_code, ImVec2(ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35))) {
            _fullscreen_pipeline.compile(_shader_code, "is0 Ray Marcher");
        }
    });
    ImGui::Begin("is0 Opt");
    _shader_code_window.open_close_checkbox();
    if (ImGui::Button("Refresh node templates")) {
        _editor.update_templates_and_nodes();
    }
    ImGui::End();
    effect_imgui_window();
}

void Is0::effect_imgui_window()
{
    ImGui::Begin("Shading");
    ImGui::Text("Fresnel");
    bool has_changed = _effects._fresnel.strenght.imgui();
    has_changed |= _effects._fresnel.col.imgui();
    ImGui::Separator();
    ImGui::Text("Glow");
    has_changed |= _effects._glow.strenght.imgui();
    has_changed |= _effects._glow.col.imgui();
    ImGui::End();
    if (has_changed) {
        _must_recompile = true;
    }
}

void Is0::on_key_pressed(const Cool::KeyboardEvent& event)
{
    if (event.action == GLFW_PRESS && Cool::Input::matches_char("a", event.key)) {
        _editor.open_menu();
    }
}