#include "Command_RemoveLink.h"
#include <ModulesGraph/ModulesGraph.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_RemoveLink)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_RemoveLink)

namespace Lab {

void Command_RemoveLink::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().remove_link(link_id);
}

auto Command_RemoveLink::to_string() const -> std::string
{
    return fmt::format("Remove link {}", reg::to_string(link_id));
}

auto Command_RemoveLink::make_reversible(MakeReversibleCommandContext_Ref const&) const -> ReversibleCommand_RemoveLink
{
    return ReversibleCommand_RemoveLink{.fwd = *this};
};

void ReversibleCommand_RemoveLink::execute(CommandExecutionContext_Ref const& ctx) const
{
    fwd.execute(ctx);
}

void ReversibleCommand_RemoveLink::revert(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().add_link(fwd.link_id, fwd.link);
}

auto ReversibleCommand_RemoveLink::to_string() const -> std::string
{
    return fwd.to_string();
}

auto ReversibleCommand_RemoveLink::merge(ReversibleCommand_RemoveLink const&) const -> std::optional<ReversibleCommand_RemoveLink>
{
    return std::nullopt;
};

} // namespace Lab