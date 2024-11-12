#include "internal_utils.h"
#include <ProjectManager/utils.h>
#include "App.h"
#include "Command_SaveProject.h"
#include "Common/Path.h"
#include "Cool/OSC/OSCManager.h"
#include "FileExtension.h"
#include "Project.h"
#include "RecentlyOpened.h"
#include "Serialization/SProject.h"
#include "utils.h"

namespace Lab::internal_project {

static void set_window_title(CommandExecutionContext_Ref const& ctx, std::optional<std::filesystem::path> const& path)
{
    glfwSetWindowTitle(
        ctx.main_window().glfw(),
        fmt::format(
            "Coollab [{}]",
            path.has_value()
                ? Cool::File::weakly_canonical(*path).string()
                : "Untitled project"
        )
            .c_str()
    );
}

void set_current_project_path(CommandExecutionContext_Ref const& ctx, std::optional<std::filesystem::path> path)
{
    if (path == Path::untitled_project() // Special case: these project paths should not be visible to the end users.
        || path == Path::backup_project())
    {
        path = std::nullopt;
    }
    Cool::Path::project_folder() = path ? std::make_optional(Cool::File::without_file_name(*path)) : std::nullopt;
    set_window_title(ctx, path);
    if (path)
        ctx.recently_opened_projects().on_project_opened(*path);
    ctx.project_path() = path;
}

void set_current_project(CommandExecutionContext_Ref const& ctx, Project&& project, std::optional<std::filesystem::path> const& project_path)
{
    bool const is_playing = ctx.project().clock.is_playing(); // Make sure the play/pause state is kept while changing project.
    before_project_destruction(ctx);

    ctx.project() = std::move(project);
    ctx.app().on_project_loaded();
    for (auto& [_, node] : ctx.project().modules_graph->graph().nodes())
        ctx.make_sure_node_uses_the_most_up_to_date_version_of_its_definition(node.downcast<Node>());

    set_current_project_path(ctx, project_path);
    ctx.project().clock.set_playing(is_playing);
    Cool::osc_manager().set_connection_endpoint(ctx.project().osc_endpoint); // HACK(OSC See below) Use the endpoint saved on the project
}

auto save_project_to(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path) -> bool
{
    ctx.project().osc_endpoint = Cool::osc_manager().get_connection_endpoint(); // HACK(OSC See above) Set the endpoint so that it will be saved in the project file
    bool const success         = do_save(ctx.project(), path);
    if (success)
    {
        if (!ctx.project_path().has_value() && path != Path::untitled_project())
        { // We just saved the untitled project as an actual project, we can delete the untitled project file so that it won't be loaded the next time we open Coollab.
            Cool::File::remove(Path::untitled_project());
        }
    }
    else
    {
        if (DebugOptions::log_project_related_events())
            Cool::Log::ToUser::info("Project", fmt::format("Failed to save project to {}.", path));
    }
    return success;
}

auto package_project_into(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& folder_path) -> bool
{
    auto file_path = folder_path;
    file_path.replace_extension(COOLLAB_FILE_EXTENSION);
    return save_project_to(ctx, file_path); // TODO(Project) Implement the packaging-specific stuff like copying images and nodes.
}

void error_when_save_failed(std::filesystem::path const& path)
{
    Cool::Log::ToUser::error(
        "Save",
        fmt::format(
            "Failed to save project in {}, please select another location.",
            path
        )
    );
}

} // namespace Lab::internal_project