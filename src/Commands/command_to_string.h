#pragma once

#include "Commands.h"

namespace Lab {

template<typename T>
auto command_to_string(const ReversibleCommand_SetValue<T> command) -> std::string
{
    return "Set " + reg::to_string(command.id) + " to " + Cool::stringify(command.value);
}

} // namespace Lab