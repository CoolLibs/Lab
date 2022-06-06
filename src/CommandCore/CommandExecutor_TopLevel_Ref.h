#pragma once

#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/ReversibleCommandMerger.h"
#include "CommandCore/make_command.h"
#include "CommandCore/try_make_reversible.h"

namespace Lab {

class CommandExecutor_TopLevel_Ref {
public:
    CommandExecutor_TopLevel_Ref(CommandExecutor_WithoutHistory_Ref sub_executor,
                                 History&                           history,
                                 VariableRegistries&                registries)
        : _sub_executor{sub_executor}
        , _history{history}
        , _variable_registries{registries}
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
        const auto reversible = try_make_reversible(command, {_variable_registries}); // Must be before the execution of the command because we need to retrieve the state of the app before execution to create the reversible command
        _sub_executor.execute(command);
        if (reversible) {
            _history.get().push(*reversible, ReversibleCommandMerger{});
        }
    }

private:
    mutable CommandExecutor_WithoutHistory_Ref _sub_executor;
    std::reference_wrapper<History>            _history;
    std::reference_wrapper<VariableRegistries> _variable_registries;
};

} // namespace Lab