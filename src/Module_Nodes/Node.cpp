#include "Node.h"
#include <stdexcept>
#include "Module_Nodes/PrimitiveType.h"

namespace Lab {

static auto type_to_template_combo_index(PrimitiveType type) -> int
{
    switch (type)
    {
#include "generated/type_to_template_combo_index.inl"
    default:
        throw std::logic_error{fmt::format("Unexpected primitive type as a template instance: {}.", static_cast<int>(type))};
    };
}

static auto template_combo_index_to_type(int index) -> PrimitiveType
{
    switch (index)
    {
#include "generated/template_combo_index_to_type.inl"
    default:
        throw std::logic_error{fmt::format("Unexpected index of primitive type as a template instance: {}.", index)};
    };
}

auto Node::imgui_chosen_any_type() -> bool
{
    if (!_chosen_any_type)
        return false;

    int index = type_to_template_combo_index(*_chosen_any_type); // We don't use *_chosen_any_type directly as an int because in the future we might want to change the order of the types in the dropdown, and we absolutely do not want to change the underlying int representation of PrimitiveType because it would break serialization.
    if (ImGui::Combo("Type", &index,
#include "generated/template_node_type_dropdown_string.inl"
        ))
    {
        *_chosen_any_type = template_combo_index_to_type(index);
        return true;
    }

    return false;
}

auto to_string(Node const& node) -> std::string
{
    return !node.name().empty() ? node.name() : node.definition_name();
}

} // namespace Lab

#include <Cool/Nodes/REGISTER_NODE_TYPE.h>
COOL_REGISTER_NODE_TYPE(Lab::Node);