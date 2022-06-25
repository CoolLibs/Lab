#pragma once

#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/ReversibleCommandMerger.h"
#include "CommandCore/make_command.h"
#include "CommandCore/try_make_reversible.h"

namespace Lab {

class CommandExecutor_TopLevel_Ref {
public:
    CommandExecutor_TopLevel_Ref(
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
    void execute(ConcreteCommandT&& command) const
    {
        execute(make_command(std::forward<ConcreteCommandT>(command)));
    }

    void execute(const Command& command) const
    {
        const auto reversible = try_make_reversible(command, _make_reversible_commands_context); // Must be before the execution of the command because we need to retrieve the state of the app before execution to create the reversible command
        _sub_executor.execute(command);
        if (reversible)
        {
            _history.get().push(*reversible, ReversibleCommandMerger{});
        }
    }

private:
    mutable CommandExecutor_WithoutHistory_Ref _sub_executor;
    std::reference_wrapper<History>            _history;
    MakeReversibleCommandContext_Ref           _make_reversible_commands_context;
};

} // namespace Lab