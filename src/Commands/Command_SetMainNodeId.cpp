#include "Command_SetMainNodeId.h"
#include <ModulesGraph/ModulesGraph.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_SetMainNodeId)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_SetMainNodeId)

namespace Lab {

void Command_SetMainNodeId::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().set_main_node_id(main_node_id);
}

auto Command_SetMainNodeId::to_string() const -> std::string
{
    return fmt::format("Set main node id to {}", reg::to_string(main_node_id));
}

auto Command_SetMainNodeId::make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_SetMainNodeId
{
    return ReversibleCommand_SetMainNodeId{
        .main_node_id     = main_node_id,
        .old_main_node_id = ctx.modules_graph().get_main_node_id(),
    };
};

void ReversibleCommand_SetMainNodeId::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().set_main_node_id(main_node_id);
}

void ReversibleCommand_SetMainNodeId::revert(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().set_main_node_id(old_main_node_id);
}

auto ReversibleCommand_SetMainNodeId::to_string() const -> std::string
{
    return fmt::format("Set main node id from {} to {}", reg::to_string(old_main_node_id), reg::to_string(main_node_id));
}

auto ReversibleCommand_SetMainNodeId::merge(ReversibleCommand_SetMainNodeId const&) const -> std::optional<ReversibleCommand_SetMainNodeId>
{
    return std::nullopt;
};

} // namespace Lab