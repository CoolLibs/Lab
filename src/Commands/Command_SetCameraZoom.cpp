#pragma once

#include "Command_SetCameraZoom.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "Dependencies/CameraManager.h"

namespace Lab {

void Command_SetCameraZoom::execute(CommandExecutionContext_Ref& ctx) const
{
    ctx.camera_manager().set_zoom(value, ctx);
}

void ReversibleCommand_SetCameraZoom::revert(CommandExecutionContext_Ref& ctx) const
{
    ctx.camera_manager().set_zoom(old_value, ctx);
}

auto Command_SetCameraZoom::make_reversible(const MakeReversibleCommandContext_Ref& ctx) const
    -> ReversibleCommand_SetCameraZoom
{
    return ReversibleCommand_SetCameraZoom{
        .forward_command = *this,
        .old_value       = ctx.camera_manager().get_zoom(),
    };
}

} // namespace Lab
