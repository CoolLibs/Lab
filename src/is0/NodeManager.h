#pragma once

#include "NodeFactory.h"

class NodeManager {
public:
    bool imgui();

private:
    NodeFactory       _factory;
    std::vector<Node> _nodes;
};