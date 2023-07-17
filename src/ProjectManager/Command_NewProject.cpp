#include "Command_NewProject.h"
#include <Project.h>
#include <optional>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "internal_utils.h"

namespace Lab {

void Command_NewProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Project) Check if the current project needs to be saved
    ctx.project()                                      = Project{};
    ctx.project().camera_manager.is_editable_in_view() = false;
    ctx.project().clock.pause();

    set_current_project_path(ctx, std::nullopt);
}

[[nodiscard]] auto Command_NewProject::to_string() const -> std::string
{
    return "Opening a new project.";
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_NewProject);
