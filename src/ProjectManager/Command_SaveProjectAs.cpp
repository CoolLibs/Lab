#include "Command_SaveProjectAs.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Serialization/Serialization.h"
#include "Project.h"
#include "cereal/archives/json.hpp"

namespace Lab {

void Command_SaveProjectAs::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Project) If the path already exists, message box to confirm that user wants to overwrite the project.
    Cool::Serialization::save<Project, cereal::JSONOutputArchive>(ctx.project(), path, "Project"); // TODO(Project) Handle error, and don't set project path if there was an error
    set_current_project_path(ctx, path);
}

[[nodiscard]] auto Command_SaveProjectAs::to_string() const -> std::string
{
    return fmt::format("Saving project as \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProjectAs);
