#include "Command_NewProject.h"
#include <Project.h>
#include <optional>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "internal_utils.h"

namespace Lab {

void Command_NewProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    auto project                                 = Project{};
    project.camera_manager.is_editable_in_view() = false;
    project.clock.pause();

    internal_project::set_current_project(ctx, std::move(project), std::nullopt /*project_path*/);
}

[[nodiscard]] auto Command_NewProject::to_string() const -> std::string
{
    return "Opening a new project.";
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_NewProject);
