#include "Command_ChangeNodeDefinition.h"
#include <ModulesGraph/ModulesGraph.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_ChangeNodeDefinition)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_ChangeNodeDefinition)

namespace Lab {

void Command_ChangeNodeDefinition::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().set_node(node_id, new_value);
}

auto Command_ChangeNodeDefinition::to_string() const -> std::string
{
    return fmt::format("Change node {} to a {} ({})", reg::to_string(node_id), new_value.definition_name(), new_value.category_name());
}

auto Command_ChangeNodeDefinition::make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_ChangeNodeDefinition
{
    auto const* old_value = ctx.modules_graph().try_get_node(node_id);
    return ReversibleCommand_ChangeNodeDefinition{.fwd = *this, .old_value = old_value ? *old_value : Node{}};
};

void ReversibleCommand_ChangeNodeDefinition::execute(CommandExecutionContext_Ref const& ctx) const
{
    fwd.execute(ctx);
}

void ReversibleCommand_ChangeNodeDefinition::revert(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().set_node(fwd.node_id, old_value);
}

auto ReversibleCommand_ChangeNodeDefinition::to_string() const -> std::string
{
    return fmt::format("Change node {} from a {} ({}) to a {} ({})", reg::to_string(fwd.node_id), old_value.definition_name(), old_value.category_name(), fwd.new_value.definition_name(), fwd.new_value.category_name());
}

auto ReversibleCommand_ChangeNodeDefinition::merge(ReversibleCommand_ChangeNodeDefinition const&) const -> std::optional<ReversibleCommand_ChangeNodeDefinition>
{
    return std::nullopt;
};

} // namespace Lab