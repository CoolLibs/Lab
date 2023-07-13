#include "Command_OpenProject.h"
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
    if (save_previous_project)
        save_project_to(ctx, ctx.project_path()); // TODO(Project) Instead, use the SaveProject command: ctx.execute(Command_SaveProject{});
    auto const error = Cool::Serialization::load<Project, cereal::JSONInputArchive>(ctx.project(), path);
    if (error)
    {
        send_error_message(error);
        ctx.project_path() = "";
        // TODO(Project) Load a default project, otherwise we could probably have corrupted state if only part of the project was loaded.
        return;
    }

    set_current_project_path(ctx, path); // Only assign path if loading was successful.
    ctx.project().is_first_frame = true;
}

[[nodiscard]] auto Command_OpenProject::to_string() const -> std::string
{
    return fmt::format("Opening project \"{}\".", std::filesystem::weakly_canonical(path).string());
}

} // namespace Lab

LAB_REGISTER_COMMAND(Lab::Command_OpenProject);
