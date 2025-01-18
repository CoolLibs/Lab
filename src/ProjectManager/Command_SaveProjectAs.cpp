#include "Command_SaveProjectAs.h"
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "ProjectManager.hpp"

namespace Lab {

void Command_SaveProjectAs::execute(CommandExecutionContext_Ref const& ctx) const
{
    ctx.project_manager().save_project_as(path, ctx.window_title_setter(), ctx.save_thumbnail());
}

[[nodiscard]] auto Command_SaveProjectAs::to_string() const -> std::string
{
    return fmt::format("Saving project as \"{}\"", Cool::File::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_SaveProjectAs);
