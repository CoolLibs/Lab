#include "Command_SaveProject.h"
#include <ProjectManager/internal_utils.h>
#include <ProjectManager/utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "Common/Path.h"

namespace Lab {

void Command_SaveProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    // Project already has a path, just save it there.
    if (ctx.project_path())
    {
        if (!save_project_to(ctx, *ctx.project_path()))
        {
            error_when_save_failed(*ctx.project_path());
            dialog_to_save_project_as(ctx.command_executor());
        }
    }
    // Autosave, save in the backup project.
    else if (is_autosave)
    {
        save_project_to(ctx, Path::backup_project());
    }
    // The user explicitly asked to save on the Unsaved Project, so run as "Save As".
    else
    {
        dialog_to_save_project_as(ctx.command_executor());
    }
}

[[nodiscard]] auto Command_SaveProject::to_string() const -> std::string
{
    return "Saving project.";
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProject);
