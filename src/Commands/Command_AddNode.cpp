#include "Command_AddNode.h"
#include <ModulesGraph/ModulesGraph.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_AddNode)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_AddNode)

namespace Lab {

void Command_AddNode::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.make_sure_node_uses_the_most_up_to_date_version_of_its_definition(node);
    ctx.modules_graph().add_node(node_id, node);
}

auto Command_AddNode::to_string() const -> std::string
{
    return fmt::format("Add node {}", reg::to_string(node_id));
}

auto Command_AddNode::make_reversible(MakeReversibleCommandContext_Ref const&) const -> ReversibleCommand_AddNode
{
    return ReversibleCommand_AddNode{.fwd = *this};
};

void ReversibleCommand_AddNode::execute(CommandExecutionContext_Ref const& ctx) const
{
    fwd.execute(ctx);
}

void ReversibleCommand_AddNode::revert(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().remove_node(fwd.node_id);
}

auto ReversibleCommand_AddNode::to_string() const -> std::string
{
    return fwd.to_string();
}

auto ReversibleCommand_AddNode::merge(ReversibleCommand_AddNode const&) const -> std::optional<ReversibleCommand_AddNode>
{
    return std::nullopt;
};

} // namespace Lab