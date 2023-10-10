#pragma once
#include <Dependencies/History.h>
#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/ReversibleCommandMerger_Ref.h"
#include "CommandCore/make_command.h"
#include "CommandCore/try_make_reversible.h"

namespace Lab {

class CommandExecutor_TopLevel {
public:
    CommandExecutor_TopLevel(
        CommandExecutor_WithoutHistory_Ref sub_executor,
        History&                           history,
        MakeReversibleCommandContext_Ref   make_reversible_commands_context
    )
        : _sub_executor{sub_executor}
        , _history{history}
        , _make_reversible_commands_context{make_reversible_commands_context}
    {
    }

    /// To simplify the life of users so they don't need to call make_command()
    template<ConcreteCommand ConcreteCommandT>
    void execute(ConcreteCommandT&& command, CommandExecutionContext_Ref const& ctx) const
    {
        execute(make_command(std::forward<ConcreteCommandT>(command)), ctx);
    }

    void execute(const Command& command, CommandExecutionContext_Ref const& ctx) const;

private:
    mutable CommandExecutor_WithoutHistory_Ref _sub_executor;
    std::reference_wrapper<History>            _history;
    MakeReversibleCommandContext_Ref           _make_reversible_commands_context;
};

} // namespace Lab