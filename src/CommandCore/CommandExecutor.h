#pragma once
#include <Commands/Command_Group.h>
#include "CommandExecutionContext_Ref.h"

namespace Lab {

class CommandExecutor {
public:
    explicit CommandExecutor(CommandExecutionContext_Ref ctx)
        : _ctx{ctx}
    {
    }

    template<ConcreteCommand ConcreteCommandT>
    void execute(ConcreteCommandT&& command) const
    {
        if (_current_group.has_value())
            _current_group->commands.push_back(make_command(std::forward<ConcreteCommandT>(command)));
        else
            _ctx.execute(std::forward<ConcreteCommandT>(command));
    }

    /// Runs the given code, and puts all the commands executed by that code in a single Command_Group.
    void wrap_in_commands_group(std::function<void()> const&);

private:
    CommandExecutionContext_Ref          _ctx;
    mutable std::optional<Command_Group> _current_group{};
};

} // namespace Lab
