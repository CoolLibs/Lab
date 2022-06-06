#pragma once

#include "CommandCore/ConcreteReversibleCommand.h"
#include "CommandCore/PolymorphicReversibleCommand.h"
#include "CommandCore/ReversibleCommand.h"

namespace Lab {

/// See explanation in make_command.h

template<ConcreteReversibleCommand ConcreteReversibleCommandT>
auto make_reversible_command(ConcreteReversibleCommandT&& concrete_reversible_command)
    -> ReversibleCommand
{
    return std::make_shared<PolymorphicReversibleCommand<ConcreteReversibleCommandT>>(
        std::forward<ConcreteReversibleCommandT>(concrete_reversible_command));
}

} // namespace Lab
