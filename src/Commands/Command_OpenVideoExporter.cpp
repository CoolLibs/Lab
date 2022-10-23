#include "Command_OpenVideoExporter.h"
#include "App.h"

namespace Lab {

void Command_OpenVideoExporter::execute(CommandExecutionContext_Ref& ctx) const
{
    ctx.app().open_video_exporter();
}

} // namespace Lab