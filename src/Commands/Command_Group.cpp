#include "Command_Group.h"
#include <sstream>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_Group)
LAB_REGISTER_REVERSIBLE_COMMAND(Lab::ReversibleCommand_Group)

namespace Lab {

void Command_Group::execute(CommandExecutionContext_Ref const& ctx) const
{
    for (auto const& command : commands)
        command->execute(ctx);
}

auto Command_Group::to_string() const -> std::string
{
    std::stringstream ss{};
    ss << "Group:";
    for (auto const& command : commands)
        ss << "\n  " << command->to_string();
    return ss.str();
}

auto Command_Group::make_reversible(MakeReversibleCommandContext_Ref const& ctx) const
    -> ReversibleCommand_Group
{
    auto res = ReversibleCommand_Group{};
    for (auto const& command : commands)
    {
        auto reversible = command->try_make_reversible(ctx);
        if (!reversible)
            continue;
        res.commands.push_back(std::move(*reversible));
    }
    return res;
}

void ReversibleCommand_Group::execute(CommandExecutionContext_Ref const& ctx) const
{
    for (auto const& command : commands)
        command->execute(ctx);
}

void ReversibleCommand_Group::revert(CommandExecutionContext_Ref const& ctx) const
{
    for (auto it = commands.rbegin(); it != commands.rend(); ++it) // We want to undo in the reverse order compared to when we do
        (*it)->revert(ctx);
}

auto ReversibleCommand_Group::to_string() const -> std::string
{
    std::stringstream ss{};
    ss << "Group:";
    for (auto const& command : commands)
        ss << "\n    " << command->to_string();
    return ss.str();
}

auto ReversibleCommand_Group::merge(ReversibleCommand_Group const&) const -> std::optional<ReversibleCommand_Group>
{
    return std::nullopt;
};

} // namespace Lab