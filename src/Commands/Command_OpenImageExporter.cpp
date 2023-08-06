#include "Command_OpenImageExporter.h"
#include "App.h"

namespace Lab {

void Command_OpenImageExporter::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.app().open_image_exporter();
}

} // namespace Lab

#include "CommandCore/LAB_REGISTER_COMMAND.h"

LAB_REGISTER_COMMAND(Lab::Command_OpenImageExporter)