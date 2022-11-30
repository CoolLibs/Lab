#include "NodesConfig.h"
#include <Cool/Dependencies/requires_shader_code_generation.h>
#include "Module_Nodes/PrimitiveType.h"

namespace Lab {

auto NodesConfig::name(Node const& node) const -> std::string
{
    return node.definition_name();
}

void NodesConfig::imgui_node_body(Node& node, Cool::NodeId const& id) const
{
    { // Main node selector
        const bool was_main = id == _main_node_id;
        bool       is_main  = was_main;
        if (ImGui::Checkbox("Main node", &is_main))
        {
            if (is_main && !was_main)
            {
                _main_node_id = id;
                _ui.set_dirty(_regenerate_code_flag);
            }
        }
    }

    if (node.imgui_chosen_any_type())
        _ui.set_dirty(_regenerate_code_flag);

    for (auto& property : node.properties())
        _ui.widget(property);
}

auto NodesConfig::make_node(NodeDefinition const& def) const -> Node
{
    auto node = Node{def.name(), def.signature().arity, def.inputs().size(), def.signature().is_template()};

    for (size_t i = 0; i < def.signature().arity; ++i)
        node.input_pins().push_back(Cool::InputPin{fmt::format("IN{}", i + 1)});
    node.output_pins().push_back(Cool::OutputPin{"OUT"});

    for (auto const& input : def.inputs())
        node.input_pins().push_back(Cool::InputPin{input.name()});

    for (auto const& property_def : def.properties())
    {
        node.properties().push_back(_input_factory.get().make(
            property_def,
            Cool::requires_shader_code_generation(property_def) ? _regenerate_code_flag : _rerender_flag
        ));
        node.input_pins().push_back(Cool::InputPin{std::visit([](auto&& property_def) { return property_def.name; }, property_def)});
    }

    for (auto const& output_index_name : def.output_indices())
        node.output_pins().push_back(Cool::OutputPin{output_index_name});

    return node;
}

} // namespace Lab
