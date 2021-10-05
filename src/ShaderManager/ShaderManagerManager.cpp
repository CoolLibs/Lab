#include "ShaderManagerManager.h"

void ShaderManagerManager::ImGui_windows()
{
    ImGui::Begin("Shader Style");
    ImGui::Checkbox("Use nodes", &_use_nodes);
    ImGui::End();
    operator*().ImGui_window();
}