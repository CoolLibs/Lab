#include "Command_SaveProjectAs.h"
#include <ProjectManager/internal_utils.h>
#include <ProjectManager/utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

void Command_SaveProjectAs::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Launcher) change uuid of the project
    if (!internal_project::save_project_to(ctx, path))
    {
        internal_project::error_when_save_failed(path);
        dialog_to_save_project_as(ctx); // Save failed, try in another location.
        return;
    }
    // Commented out because I don't like this behaviour actually: if (!ctx.project_path().has_value()) // Only change the path if we were on the unsaved project. The most common use case is: you are working on your project, then want to save a specific step, so you run a Save As. But then you want to keep working on the main project, not on the step that you saved.
    internal_project::set_current_project_path(ctx, path);
}

[[nodiscard]] auto Command_SaveProjectAs::to_string() const -> std::string
{
    return fmt::format("Saving project as \"{}\".", Cool::File::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProjectAs);
