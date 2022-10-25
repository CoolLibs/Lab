#include "Command_OpenImageExporter.h"
#include "App.h"

namespace Lab {

void Command_OpenImageExporter::execute(CommandExecutionContext_Ref& ctx) const
{
    ctx.app().open_image_exporter();
}

} // namespace Lab