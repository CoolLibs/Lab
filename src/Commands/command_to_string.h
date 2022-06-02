#pragma once

#include <stringify/stringify.hpp>
#include "Commands.h"

namespace Lab {

template<typename T>
auto command_to_string(const ReversibleCommand_SetValue<T>& command) -> std::string
{
    return "Set " + reg::to_string(command.id) + " to " + Cool::stringify(command.value);
}

template<typename T>
auto command_to_string(const Command_SetValue<T>& command) -> std::string
{
    return "Set " + reg::to_string(command.id) + " to " + Cool::stringify(command.value);
}

inline auto command_to_string(const Command_FinishedEditingValue&) -> std::string
{
    return "Finished editing value";
}

// Fallback in case we didn't find a command_to_string() for the given command type
template<typename T>
auto command_to_string(T&&) -> std::string
{
    return "CAN'T CONVERT THAT COMMAND TO A STRING, ADD A command_to_string() FUNCTION";
}

inline auto command_to_string(const Command& command) -> std::string
{
    return std::visit([](auto&& command) { return command_to_string(command); }, command);
}

} // namespace Lab