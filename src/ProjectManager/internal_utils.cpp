#include "internal_utils.h"
#include <ProjectManager/utils.h>
#include "Command_SaveProject.h"
#include "Common/Path.h"
#include "Cool/Serialization/Serialization.h"
#include "FileExtension.h"
#include "Project.h"
#include "RecentlyOpened.h"
#include "cereal/archives/json.hpp"
#include "utils.h"

namespace Lab::internal_project {

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
    if (path)
        ctx.recently_opened_projects().on_project_opened(*path);
    ctx.project_path() = path;
}

void set_current_project(CommandExecutionContext_Ref const& ctx, Project&& project, std::optional<std::filesystem::path> const& project_path)
{
    bool const is_playing = ctx.project().clock.is_playing(); // Make sure the play/pause state is kept while changing project.
    before_project_destruction(ctx);

    ctx.project() = std::move(project);
    if (project_path != Path::backup_project()) // Special case: the backup project should not be visible to the end users.
        set_current_project_path(ctx, project_path);
    ctx.project().is_first_frame = true;
    ctx.project().clock.set_playing(is_playing);
}

auto save_project_to(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path) -> bool
{
    auto const success = Cool::Serialization::save<Project, cereal::JSONOutputArchive>(ctx.project(), path, "Project");
    if (DebugOptions::log_project_related_events()
        && !success)
        Cool::Log::ToUser::info("Project", fmt::format("Failed to save project to {}.", path));
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