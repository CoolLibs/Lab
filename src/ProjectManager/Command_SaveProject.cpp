#include "Command_SaveProject.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

void Command_SaveProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    if (!ctx.project_path())
        return; // TODO(Project) We would to still want to save it on the side, for autosave, so that we can restore it if there is a crash.
    // TODO(Project) If we are on the default project, run a Save As ? (ask the user where they want to save their project) But this will be annoying with the autosave (-> we have to add a bool in this command to decide if it should trigger a Save As or not).
    save_project_to(ctx, *ctx.project_path()); // TODO(Project) handle error from saving
}

[[nodiscard]] auto Command_SaveProject::to_string() const -> std::string
{
    return "Saving project.";
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProject);
