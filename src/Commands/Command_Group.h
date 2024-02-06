#pragma once
#include "CommandCore/Command.h"
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "CommandCore/ReversibleCommand.h"

namespace Lab {

struct ReversibleCommand_Group {
    std::vector<ReversibleCommand> commands;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    void revert(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto merge(ReversibleCommand_Group const&) const -> std::optional<ReversibleCommand_Group>;
};

struct Command_Group {
    std::vector<Command> commands;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_Group;
};

} // namespace Lab

namespace cereal {

template<class Archive>
void serialize(Archive& archive, Lab::Command_Group& command)
{
    archive(
        cereal::make_nvp("Group", command.commands)
    );
}

template<class Archive>
void serialize(Archive& archive, Lab::ReversibleCommand_Group& command)
{
    archive(
        cereal::make_nvp("Group", command.commands)
    );
}

} // namespace cereal
