#include "Command_OpenVideoExporter.h"
#include "App.h"

namespace Lab {

void Command_OpenVideoExporter::execute(CommandExecutionContext_Ref& ctx) const
{
    ctx.app().open_video_exporter();
}

} // namespace Lab

#include "CommandCore/LAB_REGISTER_COMMAND.h"
LAB_REGISTER_COMMAND(Lab::Command_OpenVideoExporter)