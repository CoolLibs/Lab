#pragma once

#include <Cool/Dependencies/InputFactory_Ref.h>
#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

class NodesConfig {
public:
    NodesConfig(Cool::InputFactory_Ref const& input_factory, Cool::DirtyFlag const& dirty_flag)
        : _input_factory{input_factory}
        , _dirty_flag{dirty_flag}
    {}

    using NodeT           = Node;
    using NodeDefinitionT = NodeDefinition;

    auto name(Node const& node) const -> std::string;
    auto make_node(NodeDefinition const& def) const -> Node;

private:
    std::reference_wrapper<Cool::InputFactory_Ref const> _input_factory;
    Cool::DirtyFlag                                      _dirty_flag; // TODO(JF) Two dirty flags: some properties only affect the render, while others require the code to be regenerated
};

} // namespace Lab
