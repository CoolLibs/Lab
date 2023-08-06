#pragma once

#include "ReversibleCommand.h"

namespace Lab {

class MakeReversibleCommandContext_Ref;
class CommandExecutionContext_Ref;

class ICommand {
public:
    virtual ~ICommand() = default;

    virtual void execute(CommandExecutionContext_Ref const&) const                                                      = 0;
    virtual auto to_string() const -> std::string                                                                       = 0;
    virtual auto try_make_reversible(const MakeReversibleCommandContext_Ref&) const -> std::optional<ReversibleCommand> = 0;
};

} // namespace Lab
