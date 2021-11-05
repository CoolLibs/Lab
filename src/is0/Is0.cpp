#include "Is0.h"
#include "CodeGen.h"

void Is0::update()
{
    if (_editor.tree_has_changed()) {
        if (_editor.tree_is_valid()) {
            _shader_code = CodeGen::full_shader_code(_editor.tree(), _editor.node_templates());
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
        ImGui::Text("%s", _shader_code.c_str());
    });
    ImGui::Begin("is0 Opt");
    _shader_code_window.open_close_checkbox();
    if (ImGui::Button("Refresh node templates")) {
        _editor.update_templates_and_nodes();
    }
    ImGui::End();
}