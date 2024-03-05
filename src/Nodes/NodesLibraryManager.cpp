#include "NodesLibraryManager.h"
#include "Cool/Nodes/NodesConfig.h"
#include "Cool/Nodes/NodesDefinitionUpdater.h"
#include "Cool/Nodes/NodesGraph.h"
#include "NodesCategoryConfig.h"
#include "NodesConfig.h"
#include "parse_node_definition.h"

namespace Lab {

void NodesLibraryManager::update(Cool::DirtyFlag const& regenerate_code_flag, Cool::NodesGraph& graph, NodesConfig const& nodes_config)
{
    auto cfg = Cool::NodesConfig{nodes_config};
#if !DEBUG
    static bool is_first_frame{true};
    if (is_first_frame) // In release, only check app's Nodes folder once, to load them all. And only user-data Nodes folder is hot-reloaded.
#endif
    {
        auto updater = Cool::NodesDefinitionUpdater{cfg, graph, _nodes_library, &parse_node_definition, _nodes_folder_watcher.errors_map()};
        if (_nodes_folder_watcher.update(updater, [](std::filesystem::path const& path) { return NodesCategoryConfig{path}; }))
            regenerate_code_flag.set_dirty();
    }
    { // Must be after the base nodes, because we want the latter to create the categories first so that it will decide the order.
        auto updater = Cool::NodesDefinitionUpdater{cfg, graph, _nodes_library, &parse_node_definition, _user_nodes_folder_watcher.errors_map()};
        if (_user_nodes_folder_watcher.update(updater, [](std::filesystem::path const& path) { return NodesCategoryConfig{path}; }))
            regenerate_code_flag.set_dirty();
    }
#if !DEBUG
    is_first_frame = false;
#endif
}

} // namespace Lab