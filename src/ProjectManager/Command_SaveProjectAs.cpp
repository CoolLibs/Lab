#include "Command_SaveProjectAs.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

void Command_SaveProjectAs::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Project) If the path already exists, message box to confirm that user wants to overwrite the project.
    save_project_to(ctx, path); // TODO(Project) Handle error, and don't set project path if there was an error
    set_current_project_path(ctx, path);
}

[[nodiscard]] auto Command_SaveProjectAs::to_string() const -> std::string
{
    return fmt::format("Saving project as \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProjectAs);
