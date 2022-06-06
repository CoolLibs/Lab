#pragma once

#include "CommandCore/Command.h"
#include "CommandCore/ConcreteCommand.h"
#include "CommandCore/PolymorphicCommand.h"

namespace Lab {

/// We construct our commands by passing a copy of the command,
/// because we want to take advantage of the POD nature of structs:
/// implicitly-declared constructor, designated-initializer syntax etc.
/// Basically we want to be able to do
/// ```cpp
/// const auto cmd = make_command(
///     Command_SetValue<int>{
///         .id    = some_id,
///         .value = 3,
///     }
/// );
/// ```

template<ConcreteCommand ConcreteCommandT>
auto make_command(ConcreteCommandT&& concrete_command) -> Command
{
    return std::make_shared<PolymorphicCommand<ConcreteCommandT>>(
        std::forward<ConcreteCommandT>(concrete_command));
}

} // namespace Lab
