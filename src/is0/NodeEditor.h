#pragma once

#include "EdUniqueContext.h"
#include "Link.h"
#include "NodeFactory.h"

class NodeEditor {
public:
    void imgui_window();
    bool imgui_make_node();
    auto shader_code() const -> const std::string& { return _shader_code; }

private:
    EdUniqueContext   _context;
    NodeFactory       _factory;
    std::vector<Node> _nodes;
    std::vector<Link> _links;
    std::string       _shader_code;
};