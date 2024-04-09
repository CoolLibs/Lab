#include "Command_AddLink.h"
#include <ModulesGraph/ModulesGraph.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_AddLink)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_AddLink)

namespace Lab {

void Command_AddLink::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().add_link(link_id, link);
}

auto Command_AddLink::to_string() const -> std::string
{
    return fmt::format("Add link {}", reg::to_string(link_id));
}

auto Command_AddLink::make_reversible(MakeReversibleCommandContext_Ref const&) const -> ReversibleCommand_AddLink
{
    return ReversibleCommand_AddLink{.fwd = *this};
};

void ReversibleCommand_AddLink::execute(CommandExecutionContext_Ref const& ctx) const
{
    fwd.execute(ctx);
}

void ReversibleCommand_AddLink::revert(CommandExecutionContext_Ref const& ctx) const
{
    ctx.modules_graph().remove_link(fwd.link_id);
}

auto ReversibleCommand_AddLink::to_string() const -> std::string
{
    return fwd.to_string();
}

auto ReversibleCommand_AddLink::merge(ReversibleCommand_AddLink const&) const -> std::optional<ReversibleCommand_AddLink>
{
    return std::nullopt;
};

} // namespace Lab