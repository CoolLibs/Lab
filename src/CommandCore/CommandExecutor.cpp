#include "CommandExecutor.h"

namespace Lab {

void CommandExecutor::wrap_in_commands_group(std::function<void()> const& callback)
{
    assert(!_current_group.has_value());
    _current_group.emplace();
    callback();
    auto const group = *_current_group;
    _current_group.reset(); // We want to disable the group before executing it, otherwise we would try to append it to itself.
    execute(group);
}

} // namespace Lab