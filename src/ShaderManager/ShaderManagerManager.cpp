#include "ShaderManagerManager.h"

void ShaderManagerManager::imgui_windows()
{
    ImGui::Begin("Shader Style");
    ImGui::Checkbox("Use nodes", &_use_nodes);
    ImGui::End();
    operator*().imgui_windows();
}