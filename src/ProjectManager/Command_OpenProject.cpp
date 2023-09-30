#include "Command_OpenProject.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "Project.h"
#include "Serialization/SProject.h"
#include "internal_utils.h"

namespace Lab {

void Command_OpenProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    auto       project = Project{};
    auto const error   = deserialize(project, path);
    if (error)
    {
        error.send_error_if_any(
            [&](std::string const& /* message */) {
                return Cool::Message{
                    .category = "Loading Project failed",
                    .message  = fmt::format(
                        "Incompatible version. Use Coollab **{}** instead. You can download it from [https://github.com/CoolLibs/Lab/releases](https://github.com/CoolLibs/Lab/releases).",
                        project.debug_info_coollab_version
                    ),
                    .severity = Cool::MessageSeverity::Warning,
                };
            },
            Cool::Log::ToUser::console()
        );
        return;
    }

    internal_project::set_current_project(ctx, std::move(project), path);
}

[[nodiscard]] auto Command_OpenProject::to_string() const -> std::string
{
    return fmt::format("Opening project \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_OpenProject);
