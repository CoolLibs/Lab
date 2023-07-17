#include "internal_utils.h"
#include <ProjectManager/utils.h>
#include "Command_SaveProject.h"
#include "Cool/Serialization/Serialization.h"
#include "Project.h"
#include "cereal/archives/json.hpp"

namespace Lab {

static void set_window_title(CommandExecutionContext_Ref const& ctx, std::optional<std::filesystem::path> const& path)
{
    glfwSetWindowTitle(
        ctx.main_window().glfw(),
        fmt::format(
            "Coollab [{}]",
            path.has_value()
                ? std::filesystem::weakly_canonical(*path).string()
                : "Unsaved project"
        )
            .c_str()
    );
}

void set_current_project_path(CommandExecutionContext_Ref const& ctx, std::optional<std::filesystem::path> const& path)
{
    set_window_title(ctx, path);
    ctx.project_path() = path;
}

void set_current_project(CommandExecutionContext_Ref const& ctx, Project&& project, std::optional<std::filesystem::path> const& project_path)
{
    if (ctx.project_path().has_value())
    {
        ctx.execute(Command_SaveProject{});
    }
    else
    {
        while (true) // If the user cancels the save dialog, we want to ask again if they want to save or not. This will prevent closing the dialog by mistake and then losing your changes.
        {
            if (boxer::show("You have unsaved changes. Do you want to save them? They will be lost otherwise.", "Unsaved project", boxer::Style::Warning, boxer::Buttons::YesNo)
                != boxer::Selection::Yes)
            {
                break;
            }
            if (dialog_to_save_project_as(ctx.command_executor()))
                break;
        }
    }

    ctx.project() = std::move(project);
    set_current_project_path(ctx, project_path);
    ctx.project().is_first_frame = true;
}

void save_project_to(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path)
{
    Cool::Serialization::save<Project, cereal::JSONOutputArchive>(ctx.project(), path, "Project");
}

} // namespace Lab