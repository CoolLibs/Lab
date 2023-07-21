#include "NodesLibraryManager.h"
#include <Module_Nodes/NodesCategoryConfig.h>
#include <Module_Nodes/NodesConfig.h>
#include <Module_Nodes/parse_node_definition.h>
#include "Cool/Nodes/Graph.h"
#include "Cool/Nodes/NodesConfig.h"
#include "Cool/Nodes/NodesDefinitionUpdater.h"
#include "NodesConfig.h"

namespace Lab {

void NodesLibraryManager::update(UpdateContext_Ref ctx, Cool::DirtyFlag& regenerate_code_flag, Cool::Graph& graph, NodesConfig const& nodes_config)
{
    auto cfg     = Cool::NodesConfig{nodes_config};
    auto updater = Cool::NodesDefinitionUpdater{cfg, graph, _nodes_library, &parse_node_definition, _nodes_folder_watcher.errors_map()};
    if (_nodes_folder_watcher.update(updater, [](std::filesystem::path const& path) { return NodesCategoryConfig{path}; }))
        ctx.set_dirty(regenerate_code_flag);
}

} // namespace Lab