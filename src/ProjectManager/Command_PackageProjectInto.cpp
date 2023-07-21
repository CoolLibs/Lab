#include "Command_PackageProjectInto.h"
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "internal_utils.h"

namespace Lab {

void Command_PackageProjectInto::execute(CommandExecutionContext_Ref const& ctx) const
{
    package_project_into(ctx, folder_path);
}

[[nodiscard]] auto Command_PackageProjectInto::to_string() const -> std::string
{
    return fmt::format("Packaging project into {}.", folder_path);
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_PackageProjectInto);
