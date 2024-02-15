#include "Command_RemoveNode.h"
#include <ModulesGraph/ModulesGraph.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"
#include "Cool/Nodes/as_ed_id.h"
#include "Cool/Nodes/ed.h"

LAB_REGISTER_COMMAND(Lab::Command_RemoveNode)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_RemoveNode)

namespace Lab {

void Command_RemoveNode::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().remove_node(node_id);
}

auto Command_RemoveNode::to_string() const -> std::string
{
    return fmt::format("Remove node {}", reg::to_string(node_id));
}

auto Command_RemoveNode::make_reversible(MakeReversibleCommandContext_Ref const&) const -> ReversibleCommand_RemoveNode
{
    return ReversibleCommand_RemoveNode{
        .fwd      = *this,
        .node_pos = ed::GetNodePosition(Cool::as_ed_id(node_id)),
    };
};

void ReversibleCommand_RemoveNode::execute(CommandExecutionContext_Ref const& ctx) const
{
    node_pos = ed::GetNodePosition(Cool::as_ed_id(fwd.node_id)); // Get the pos each time we remove the node, because it might have changed in-between
    fwd.execute(ctx);
}

void ReversibleCommand_RemoveNode::revert(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().add_node(fwd.node_id, fwd.node);
    ed::SetNodePosition(Cool::as_ed_id(fwd.node_id), node_pos);
}

auto ReversibleCommand_RemoveNode::to_string() const -> std::string
{
    return fwd.to_string();
}

auto ReversibleCommand_RemoveNode::merge(ReversibleCommand_RemoveNode const&) const -> std::optional<ReversibleCommand_RemoveNode>
{
    return std::nullopt;
};

} // namespace Lab