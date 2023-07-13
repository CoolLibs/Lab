#include "Command_SaveProject.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

void Command_SaveProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Project) If we are on the default project, run a Save As ? (ask the user where they want to save their project) But this will be annoying with the autosave.
    save_project_to(ctx, ctx.project_path()); // TODO(Project) handle error from saving
}

[[nodiscard]] auto Command_SaveProject::to_string() const -> std::string
{
    return "Saving project.";
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProject);
