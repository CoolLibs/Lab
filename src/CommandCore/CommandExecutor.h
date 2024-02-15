#pragma once
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
        _ctx.execute(std::forward<ConcreteCommandT>(command));
    }

private:
    CommandExecutionContext_Ref _ctx;
};

} // namespace Lab
