#include "internal_utils.h"
#include <ProjectManager/utils.h>
#include "Command_SaveProject.h"
#include "Common/Path.h"
#include "Cool/Serialization/Serialization.h"
#include "Project.h"
#include "cereal/archives/json.hpp"
#include "utils.h"

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
    before_project_destruction(ctx);

    ctx.project() = std::move(project);
    if (project_path != Path::backup_project()) // Special case: the backup project should not be visible to the end users.
        set_current_project_path(ctx, project_path);
    ctx.project().is_first_frame = true;
}

void save_project_to(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path)
{
    Cool::Serialization::save<Project, cereal::JSONOutputArchive>(ctx.project(), path, "Project");
}

} // namespace Lab