#include "Node.h"

namespace Lab {

auto Node::imgui_chosen_any_type() -> bool
{
    if (!_chosen_any_type)
        return false;

    return ImGui::Combo("Type", reinterpret_cast<int*>(&*_chosen_any_type),
#include "generated/template_node_type_dropdown_string.inl"
    );
}

} // namespace Lab