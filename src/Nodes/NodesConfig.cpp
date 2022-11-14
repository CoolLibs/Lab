#include "NodesConfig.h"

namespace Lab {

auto NodesConfig::name(Node const& node) const -> std::string
{
    return node.definition_name();
}

void NodesConfig::imgui_node_body(Node& node) const
{
    for (auto& property : node.properties())
        _ui.widget(property);
}

auto NodesConfig::make_node(NodeDefinition const& def) const -> Node
{
    auto node = Node{def.name()};

    node.input_pins().push_back(Cool::InputPin{"IN"});
    node.output_pins().push_back(Cool::OutputPin{"OUT"});

    for (auto const& property_def : def.properties)
        node.properties().push_back(_input_factory.get().make(property_def, _dirty_flag));

    return node;
}

} // namespace Lab
