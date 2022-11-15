#pragma once

#include <Cool/Nodes/NodesLibrary.h>
#include "NodeDefinition.h"

namespace Lab {

class NodesLibrary : public Cool::NodesLibrary<NodeDefinition> {
public:
    NodesLibrary();
};

} // namespace Lab