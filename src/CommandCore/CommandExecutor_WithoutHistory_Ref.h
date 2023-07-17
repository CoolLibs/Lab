#pragma once

#include "CommandCore/Command.h"
#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/ConcreteReversibleCommand.h"
#include "CommandCore/ReversibleCommand.h"
#include "CommandCore/make_command.h"
#include "CommandCore/make_reversible_command.h"
#include "Debug/DebugOptions.h"
#include "IReversibleCommand.h"

namespace Lab {

class CommandExecutionContext_Ref;

class CommandExecutor_WithoutHistory_Ref {
public:
    // To simplify the life of users so they don't need to call make_command()
    template<ConcreteCommand ConcreteCommandT>
    void execute(ConcreteCommandT&& command, CommandExecutionContext_Ref const& context)
    {
        execute(make_command(std::forward<ConcreteCommandT>(command)), context);
    }

    void execute(const Command& command, CommandExecutionContext_Ref const& context);
};

} // namespace Lab