#pragma once

#include "CommandCore/Command.h"
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/CommandLogger.h"
#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/ConcreteReversibleCommand.h"
#include "CommandCore/ReversibleCommand.h"
#include "CommandCore/make_command.h"
#include "CommandCore/make_reversible_command.h"
#include "IReversibleCommand.h"

namespace Lab {

class CommandExecutor_WithoutHistory_Ref {
public:
    explicit CommandExecutor_WithoutHistory_Ref(CommandExecutionContext_Ref context,
                                                CommandLogger&              command_logger)
        : _context{context}
        , _command_logger{command_logger}
    {
    }
    /// To simplify the life of users so they don't need to call make_command()
    template<ConcreteCommand ConcreteCommandT>
    void execute(ConcreteCommandT&& command)
    {
        execute(make_command(std::forward<ConcreteCommandT>(command)));
    }

    void execute(const Command& command)
    {
        command->execute(_context);
        _command_logger.get().push(command);
    }

private:
    CommandExecutionContext_Ref           _context;
    std::reference_wrapper<CommandLogger> _command_logger;
};

class ReversibleCommandExecutor_WithoutHistory_Ref {
public:
    explicit ReversibleCommandExecutor_WithoutHistory_Ref(CommandExecutionContext_Ref context,
                                                          CommandLogger&              command_logger)
        : _context{context}
        , _command_logger{command_logger}
    {
    }

    /// To simplify the life of users so they don't need to call make_reversible_command()
    template<ConcreteReversibleCommand ConcreteReversibleCommandT>
    void execute(ConcreteReversibleCommandT&& command) const
    {
        execute(make_reversible_command(std::forward<ConcreteReversibleCommandT>(command)));
    }

    /// To simplify the life of users so they don't need to call make_reversible_command()
    template<ConcreteReversibleCommand ConcreteReversibleCommandT>
    void revert(ConcreteReversibleCommandT&& command) const
    {
        revert(make_reversible_command(std::forward<ConcreteReversibleCommandT>(command)));
    }

    void execute(const ReversibleCommand& command)
    {
        command->execute(_context);
        // _command_logger.get().push(command); // TODO support the reversible commands in the logger
    }

    void revert(const ReversibleCommand& command)
    {
        command->revert(_context);
        // _command_logger.get().push(command); // TODO support the reversible commands in the logger
    }

private:
    CommandExecutionContext_Ref           _context;
    std::reference_wrapper<CommandLogger> _command_logger;
};

} // namespace Lab