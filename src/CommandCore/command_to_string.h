#pragma once

#include "CommandCore/Command.h"
#include "CommandCore/IReversibleCommand.h"
#include "CommandCore/ReversibleCommand.h"

namespace Lab {

inline auto command_to_string(const Command& command) -> std::string
{
    return command->to_string();
}

inline auto command_to_string(const ReversibleCommand& command) -> std::string
{
    return command->to_string();
}

} // namespace Lab