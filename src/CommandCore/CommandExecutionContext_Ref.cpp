#include "CommandExecutionContext_Ref.h"
#include "CommandExecutor.h"

namespace Lab {

auto CommandExecutionContext_Ref::command_executor() const -> CommandExecutor
{
    return CommandExecutor{*this};
}

} // namespace Lab