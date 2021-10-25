#pragma once

#include "EdUniqueContext.h"
#include "Link.h"
#include "NodeFactory.h"

class NodeManager {
public:
    void imgui_window();
    bool imgui_make_node();

private:
    EdUniqueContext   _context;
    NodeFactory       _factory;
    std::vector<Node> _nodes;
    std::vector<Link> _links;
};