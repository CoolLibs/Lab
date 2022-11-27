#pragma once

#include <Cool/Nodes/NodesLibrary.h>
#include "NodeDefinition.h"

namespace Lab {

class NodesLibrary : public Cool::NodesLibrary<NodeDefinition> {
public:
    NodesLibrary();

    void add_node_definition(NodeDefinition_Data definition);
};

} // namespace Lab