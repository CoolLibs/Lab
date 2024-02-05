#pragma once

#include "ICommand.h"

namespace Lab {

/// A shared pointer to const is equivalent to a value type, we can copy it around freely
/// so the cmd library will be able to use it without problem.

/// We use inheritance for our commands because we want plugins to be able to create their own commands,
/// so we don't know all the commands at compile time and can't use a std::variant.

using Command = std::shared_ptr<ICommand const>;

} // namespace Lab
