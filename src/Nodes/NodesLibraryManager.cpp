#include "NodesLibraryManager.h"
#include "Cool/Nodes/NodesConfig.h"
#include "Cool/Nodes/NodesDefinitionUpdater.h"
#include "Cool/Nodes/NodesGraph.h"
#include "NodesCategoryConfig.h"
#include "NodesConfig.h"
#include "parse_node_definition.h"

namespace Lab {

void NodesLibraryManager::update(UpdateContext_Ref ctx, Cool::DirtyFlag& regenerate_code_flag, Cool::NodesGraph& graph, NodesConfig const& nodes_config)
{
    auto cfg     = Cool::NodesConfig{nodes_config};
    auto updater = Cool::NodesDefinitionUpdater{cfg, graph, _nodes_library, &parse_node_definition, _nodes_folder_watcher.errors_map()};
    if (_nodes_folder_watcher.update(updater, [](std::filesystem::path const& path) { return NodesCategoryConfig{path}; }))
        ctx.set_dirty(regenerate_code_flag);
}

} // namespace Lab