#include "Command_SaveProjectAs.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

void Command_SaveProjectAs::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Project) If the path already exists, message box to confirm that user wants to overwrite the project.
    save_project_to(ctx, path);          // TODO(Project) Handle error, and don't set project path if there was an error
    set_current_project_path(ctx, path); // TODO(Project) Only change the path if we were on the default project ? I think so most common use case is: you are working on your project, then want to save a specific step, so you run as Save As. But then you want to keep working on the main project, not the step that you saved.
}

[[nodiscard]] auto Command_SaveProjectAs::to_string() const -> std::string
{
    return fmt::format("Saving project as \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProjectAs);