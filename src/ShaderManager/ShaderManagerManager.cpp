#include "ShaderManagerManager.h"

void ShaderManagerManager::imgui_windows()
{
    ImGui::Begin("Shader Style");
    ImGui::Checkbox("Use nodes", &_use_nodes);
    ImGui::End();
    operator*().imgui_windows();
}

void ShaderManagerManager::add_node(Node node)
{
    _is0.add_node(node);
}

const std::vector<NodeTemplate>& ShaderManagerManager::nodes()
{
    return _is0.nodes();
}