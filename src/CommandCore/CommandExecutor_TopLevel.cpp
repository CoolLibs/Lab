#include "CommandExecutor_TopLevel.h"
#include "CommandExecutionContext_Ref.h"

namespace Lab {

void CommandExecutor_TopLevel::execute(const Command& command, CommandExecutionContext_Ref const& ctx, bool store_in_history) const
{
    if (store_in_history)
    {
        auto const reversible = try_make_reversible(command, _make_reversible_commands_context); // Must be before the execution of the command because we need to retrieve the state of the app before execution to create the reversible command
        _sub_executor.execute(command, ctx);
        if (reversible)
            _history.get().push(*reversible, ReversibleCommandMerger_Ref{});
    }
    else
    {
        _sub_executor.execute(command, ctx);
    }
}

} // namespace Lab