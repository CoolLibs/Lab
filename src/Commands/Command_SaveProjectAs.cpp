#include <Commands/Command_SaveProjectAs.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Serialization/Serialization.h"
#include "Project.h"
#include "cereal/archives/json.hpp"

namespace Lab {

void Command_SaveProjectAs::execute(CommandExecutionContext_Ref const& ctx) const
{
    // TODO(Project) If the path already exists, message box to confirm that user wants to overwrite the project.
    ctx.project_path() = path;
    Cool::Serialization::save<Project, cereal::JSONOutputArchive>(ctx.project(), path, "Project");
}

[[nodiscard]] auto Command_SaveProjectAs::to_string() const -> std::string
{
    return fmt::format("Saving project as \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProjectAs);
