#pragma once

#include "Node.h"

namespace Lab {

struct NodesConfig {
    using NodeT = Node;
    static auto name(Node const&) -> const char* { return "Some Node"; }
};

} // namespace Lab
