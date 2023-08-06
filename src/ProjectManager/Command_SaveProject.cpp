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
        if (!internal_project::save_project_to(ctx, *ctx.project_path()))
        {
            internal_project::error_when_save_failed(*ctx.project_path());
            dialog_to_save_project_as(ctx);
        }
    }
    // Autosave, save in the backup project.
    else if (is_autosave)
    {
        internal_project::save_project_to(ctx, Path::backup_project());
    }
    // The user explicitly asked to save on the Unsaved Project, so run as "Save As".
    else
    {
        dialog_to_save_project_as(ctx);
    }
}

[[nodiscard]] auto Command_SaveProject::to_string() const -> std::string
{
    return "Saving project.";
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProject);
