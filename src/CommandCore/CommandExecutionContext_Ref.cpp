#include "CommandExecutionContext_Ref.h"
#include "App.h"
#include "CommandExecutor.h"
#include "Nodes/make_sure_node_uses_the_most_up_to_date_version_of_its_definition.h"

namespace Lab {

auto CommandExecutionContext_Ref::command_executor() const -> CommandExecutor
{
    return CommandExecutor{*this};
}

void CommandExecutionContext_Ref::make_sure_node_uses_the_most_up_to_date_version_of_its_definition(Node& node) const
{
    Lab::make_sure_node_uses_the_most_up_to_date_version_of_its_definition(
        node,
        Cool::GetNodeDefinition_Ref<NodeDefinition>{app().nodes_library()},
        app().project().modules_graph->graph(),
        command_executor(),
        Cool::GetNodeCategoryConfig_Ref{app().nodes_library()},
        app().project().modules_graph->dirty_flags()
    );
}

} // namespace Lab