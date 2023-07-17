#include "Command_OpenProject.h"
#include <ProjectManager/internal_utils.h>
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Serialization/Serialization.h"
#include "Project.h"
#include "cereal/archives/json.hpp"
#include "internal_utils.h"

namespace Lab {

static void send_error_message(Cool::OptionalErrorMessage const& error)
{
    error.send_error_if_any(
        [&](std::string const& message) {
            return Cool::Message{
                .category = "Loading Project",
                .message  = message,
                .severity = Cool::MessageSeverity::Warning,
            };
        },
        Cool::Log::ToUser::console()
    );
}

void Command_OpenProject::execute(CommandExecutionContext_Ref const& ctx) const
{
    auto       project = Project{};
    auto const error   = Cool::Serialization::load<Project, cereal::JSONInputArchive>(project, path);
    if (error)
    {
        send_error_message(error);
        return;
    }

    set_current_project(ctx, std::move(project), path);
}

[[nodiscard]] auto Command_OpenProject::to_string() const -> std::string
{
    return fmt::format("Opening project \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_OpenProject);
