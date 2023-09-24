#include "Command_OpenBackupProject.h"
#include <filesystem>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "Command_OpenProject.h"
#include "Common/Path.h"
#include "Cool/Log/ToUser.h"

namespace Lab {

void Command_OpenBackupProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    auto const path = Path::backup_project();
    if (!std::filesystem::exists(path))
    {
        Cool::Log::ToUser::warning("Loading backup project failed", "No backup found.");
        return;
    }
    ctx.execute(Command_OpenProject{.path = path});
}

[[nodiscard]] auto Command_OpenBackupProject::to_string() const -> std::string
{
    return fmt::format("Opening backup project.");
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_OpenBackupProject);
