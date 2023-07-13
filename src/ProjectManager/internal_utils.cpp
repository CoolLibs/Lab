#include "internal_utils.h"
#include "Cool/Serialization/Serialization.h"
#include "Project.h"
#include "cereal/archives/json.hpp"

namespace Lab {

static void set_window_title(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path)
{
    glfwSetWindowTitle(
        ctx.main_window().glfw(),
        fmt::format(
            "Coollab [{}]",
            std::filesystem::weakly_canonical(path).string()
        )
            .c_str()
    );
}

void set_current_project_path(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path)
{
    set_window_title(ctx, path);
    ctx.project_path() = path;
}

void save_project_to(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path)
{
    Cool::Serialization::save<Project, cereal::JSONOutputArchive>(ctx.project(), path, "Project");
}

} // namespace Lab