#pragma once
#include <CommandCore/CommandExecutor.h>
#include <Nodes/NodeDefinition.h>
#include "Cool/Nodes/GetNodeCategoryConfig.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Cool/Nodes/NodesGraph.h"
#include "ModulesGraph/DirtyFlags.h"
#include "Node.h"

namespace Lab {

void make_sure_node_uses_the_most_up_to_date_version_of_its_definition(Node& node, Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition, Cool::NodesGraph& graph, CommandExecutor const& command_executor, Cool::GetNodeCategoryConfig_Ref get_node_category_config, DirtyFlags const& dirty_flags);

} // namespace Lab