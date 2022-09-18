#pragma once
#include <filesystem>
#include "Node.h"
#include "NodeDefinition.h"

namespace Lab {

class NodeLibrary {
public:
    auto get_definition(std::string_view definition_name) const -> const NodeDefinition*;

    void clear();
    void add_nodes_from(std::filesystem::path);

private:
    std::vector<NodeDefinition> _definitions;
    // TODO(JF) Folder Watcher, or at file watcher for each current node to auto refresh it
};

} // namespace Lab