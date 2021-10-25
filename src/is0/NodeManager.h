#pragma once

#include "EdUniqueContext.h"
#include "NodeFactory.h"

class NodeManager {
public:
    bool imgui();

private:
    EdUniqueContext   _context;
    NodeFactory       _factory;
    std::vector<Node> _nodes;
};