#pragma once

#include "ReversibleCommand.h"

namespace Lab {

class CommandExecutionContext_Ref;

class IReversibleCommand {
public:
    virtual ~IReversibleCommand() = default;

    virtual void execute(CommandExecutionContext_Ref&) const                                         = 0;
    virtual void revert(CommandExecutionContext_Ref&) const                                          = 0;
    virtual auto to_string() const -> std::string                                                    = 0; // TODO(JF) have a to_string_forward and to_string_backward (NB: find better names) to convert to string the execution and the revert of the command
    virtual auto merge(const IReversibleCommand& previous) const -> std::optional<ReversibleCommand> = 0;
};

} // namespace Lab
