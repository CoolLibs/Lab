#include "CommandExecutor_WithoutHistory_Ref.h"
#include <Cool/Log/ToUser.h>
#include "CommandExecutionContext_Ref.h"

namespace Lab {

void CommandExecutor_WithoutHistory_Ref::execute(const Command& command, CommandExecutionContext_Ref const& context)
{
    command->execute(context);
    if (DebugOptions::log_when_executing_a_command())
        Cool::Log::ToUser::info("Command", command->to_string());
}

} // namespace Lab