#include "CommandExecutionContext_Ref.h"
#include "App.h"
#include "CommandExecutor.h"
#include "Nodes/make_sure_node_uses_the_most_up_to_date_version_of_its_definition.h"

namespace Lab {

auto CommandExecutionContext_Ref::history() const -> History const&
{
    return _data.project.get().history;
}
auto CommandExecutionContext_Ref::camera_manager() const -> Camera3DManager&
{
    return _data.project.get().camera_3D_manager;
}
auto CommandExecutionContext_Ref::command_executor() const -> CommandExecutor
{
    return CommandExecutor{*this};
}
auto CommandExecutionContext_Ref::modules_graph() const -> ModulesGraph&
{
    return *_data.project.get().modules_graph;
}

void CommandExecutionContext_Ref::make_sure_node_uses_the_most_up_to_date_version_of_its_definition(Node& node) const
{
    Lab::make_sure_node_uses_the_most_up_to_date_version_of_its_definition(
        node,
        Cool::GetNodeDefinition_Ref<NodeDefinition>{app().nodes_library()},
        modules_graph().graph(),
        command_executor(),
        Cool::GetNodeCategoryConfig_Ref{app().nodes_library()},
        modules_graph().dirty_flags()
    );
}

} // namespace Lab