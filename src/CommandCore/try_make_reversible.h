#pragma once

#include "CommandCore/Command.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {

inline auto try_make_reversible(const Command& cmd, const MakeReversibleCommandContext_Ref& ctx)
    -> std::optional<ReversibleCommand>
{
    return cmd->try_make_reversible(ctx);
}

} // namespace Lab
