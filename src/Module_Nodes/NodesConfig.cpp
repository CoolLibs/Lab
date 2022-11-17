#include "NodesConfig.h"
#include <Cool/Dependencies/requires_shader_code_generation.h>

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

    for (auto& property : node.properties())
        _ui.widget(property);
}

auto NodesConfig::make_node(NodeDefinition const& def) const -> Node
{
    auto node = Node{def.name()};

    node.input_pins().push_back(Cool::InputPin{"IN"});
    node.output_pins().push_back(Cool::OutputPin{"OUT"});

    for (auto const& input : def.inputs())
        node.input_pins().push_back(Cool::InputPin{input.name()});

    for (auto const& property_def : def.properties())
    {
        node.properties().push_back(_input_factory.get().make(
            property_def,
            Cool::requires_shader_code_generation(property_def) ? _regenerate_code_flag : _rerender_flag
        ));
    }

    return node;
}

} // namespace Lab
