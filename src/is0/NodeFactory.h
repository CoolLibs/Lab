#pragma once

#include "Node.h"
#include "NodeTemplate.h"

struct NodeTemplatesFolder {
    std::string name;
    size_t      nodes_count{};
};

class NodeFactory {
public:
    explicit NodeFactory(std::string_view nodes_folder_path);
    std::optional<Node>              imgui();
    const std::vector<NodeTemplate>& templates() const { return _node_templates; }
    void                             reload_templates();

private:
    std::vector<NodeTemplate>        _node_templates;
    std::vector<NodeTemplatesFolder> _folders;
    std::string                      _nodes_folder_path;
};

namespace NodeFactoryU {

Node node_from_template(const NodeTemplate& node_template);

}