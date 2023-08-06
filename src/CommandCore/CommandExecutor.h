#pragma once
#include "CommandExecutionContext_Ref.h"

namespace Lab {

class CommandExecutor {
public:
    explicit CommandExecutor(CommandExecutionContext_Ref ctx)
        : _ctx{std::move(ctx)}
    {
    }

    template<ConcreteCommand ConcreteCommandT>
    void execute(ConcreteCommandT&& command) const
    {
        _ctx.execute(command);
    }

private:
    CommandExecutionContext_Ref _ctx;
};

} // namespace Lab
