#include "NodesConfig.h"

namespace Lab {

auto NodesConfig::name(Node const& node) -> std::string
{
    return node.definition_name();
}

auto NodesConfig::make_node(NodeDefinition const& def) -> Node
{
    auto node = Node{def.name()};

    // for (auto const& )
    node.input_pins().push_back(Cool::InputPin{"IN"});
    node.output_pins().push_back(Cool::OutputPin{"OUT"});

    return node;
}

} // namespace Lab
