#pragma once

#include "Node.h"
#include "NodeTemplate.h"

struct NodeTemplatesFolder {
    std::string name;
    size_t      nodes_count{};
};

struct FolderRange {
    size_t first;
    size_t last;
};

class NodeFactory {
public:
    explicit NodeFactory(std::filesystem::path nodes_folder_path);
    std::optional<Node>              imgui();
    const std::vector<NodeTemplate>& templates() const { return _node_templates; }
    std::optional<FolderRange>       templates_from_folder(std::string_view folder_name) const;
    void                             reload_templates();
    void                             clear_filter();

private:
    std::vector<NodeTemplate>        _node_templates;
    std::vector<NodeTemplatesFolder> _folders;
    std::filesystem::path            _nodes_folder_path;
    std::string                      _filter;
    bool                             _focus_on_filter = true;
};

namespace NodeFactoryU {

Node node_from_template(const NodeTemplate& node_template);

}