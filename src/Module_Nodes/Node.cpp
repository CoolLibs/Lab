#include "Node.h"
#include "Module_Nodes/PrimitiveType.h"

namespace Lab {

static auto type_to_template_combo_index(PrimitiveType type) -> int
{
    switch (type)
    {
#include "generated/type_to_template_combo_index.inl"
    };
}

static auto template_combo_index_to_type(int index) -> PrimitiveType
{
    switch (index)
    {
#include "generated/template_combo_index_to_type.inl"
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

} // namespace Lab