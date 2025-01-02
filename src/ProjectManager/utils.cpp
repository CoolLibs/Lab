#include "utils.h"
#include <ImGuiNotify/ImGuiNotify.hpp>
#include <filesystem>
#include "App.h"
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "Command_SaveProject.h"
#include "Command_SaveProjectAs.h"
#include "Common/Path.h"
#include "Cool/CommandLineArgs/CommandLineArgs.h"
#include "Cool/File/File.h"
#include "Cool/OSC/OSCManager.h"
#include "FileExtension.h"
#include "Project.h"
#include "Serialization/SProject.h"
#include "filesystem"
#include "internal_utils.h"

namespace Lab {

static void set_current_project(CommandExecutionContext_Ref const& ctx, Project project, std::optional<std::filesystem::path> const& project_path)
{
    ctx.project() = std::move(project);
    ctx.app().on_project_loaded();
    for (auto& [_, node] : ctx.project().modules_graph->graph().nodes())
        ctx.make_sure_node_uses_the_most_up_to_date_version_of_its_definition(node.downcast<Node>());

    internal_project::set_current_project_path(ctx, project_path);
    Cool::osc_manager().set_connection_endpoint(ctx.project().osc_endpoint); // HACK(OSC See below) Use the endpoint saved on the project
}

static void create_new_project(CommandExecutionContext_Ref const& ctx)
{
    auto project                                    = Project{};
    project.camera_3D_manager.is_editable_in_view() = false;
    project.clock.pause();
    set_current_project(ctx, std::move(project), std::nullopt /*project_path*/);
}

[[nodiscard]] static auto try_load_project(CommandExecutionContext_Ref const& ctx, std::filesystem::path const& path) -> bool
{
    auto       project = Project{};
    auto const error   = do_load(project, path);
    if (error)
    {
        ImGuiNotify::send({
            .type     = ImGuiNotify::Type::Error,
            .title    = fmt::format("Failed to open project \"{}\"", Cool::File::file_name_without_extension(path)),
            .content  = *error.error_message(),
            .duration = std::nullopt,
        });
        return false;
    }

    set_current_project(ctx, std::move(project), path);
    return true;
}

void initial_project_opening(CommandExecutionContext_Ref const& ctx)
{
    if (Cool::command_line_args().get().empty())
    {
        create_new_project(ctx);
    }
    else
    {
        auto const success = try_load_project(ctx, Cool::command_line_args().get()[0]); // Load the project that was requested, e.g. when selecting a project from the launcher
        if (!success)
            create_new_project(ctx);
    }
}

static auto project_dialog_args(CommandExecutionContext_Ref const& /* ctx */) -> Cool::File::file_dialog_args
{
    // TODO(Launcher)
    // auto initial_folder = ctx.recently_opened_projects().most_recent_path(); // If a project is currently open this will be its path, else it will be the path of the last open one.
    // if (initial_folder)
    //     initial_folder = initial_folder->parent_path(); // Remove the file part, keep only the folder.
    // else
    std::optional<std::string> initial_folder = "";

    return Cool::File::file_dialog_args{
        .file_filters   = {{"Coollab project", COOLLAB_FILE_EXTENSION}},
        .initial_folder = *initial_folder,
    };
}

auto dialog_to_save_project_as(CommandExecutionContext_Ref const& ctx) -> bool
{
    auto const path = Cool::File::file_saving_dialog(project_dialog_args(ctx));
    if (!path)
        return false;

    ctx.execute(Command_SaveProjectAs{
        .path = *path,
    });
    return true;
}

void before_project_destruction(CommandExecutionContext_Ref const& ctx)
{
    if (ctx.project_path().has_value())
    {
        ctx.execute(Command_SaveProject{});
        return;
    }
    if (ctx.project().is_empty())
        return;

    // TODO(Launcher) We should always save the project
    // We are on an untitled project, ask to save it.
    bool has_saved{false};
    while (true) // If the user cancels the save dialog, we want to ask again if they want to save or not. This will prevent closing the dialog by mistake and then losing your changes.
    {
        if (boxer::show("You have unsaved changes. Do you want to save them? They will be lost otherwise.\n(NB: Actually you can still recover them by using \"Open Backup\" in the \"File\" menu.)", "Unsaved project", boxer::Style::Warning, boxer::Buttons::YesNo)
            != boxer::Selection::Yes)
        {
            break;
        }
        if (dialog_to_save_project_as(ctx))
        {
            has_saved = true;
            break;
        }
    }
    Cool::File::remove_file(Path::untitled_project()); // Users either saved their untitled project, or accepted to lose their changes.
    if (!has_saved)
        internal_project::save_project_to(ctx, Path::backup_project());
}

void imgui_menu_items_to_save_project(CommandExecutionContext_Ref const& ctx)
{
    if (ImGui::MenuItem("Save", "Ctrl+S")) // TODO(UX) Cmd instead of Ctrl on MacOS
        ctx.execute(Command_SaveProject{});
    if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        dialog_to_save_project_as(ctx);
}

static auto path_to_launcher_project_info() -> std::filesystem::path
{
    return "C:/Dev/Cool/launcher/User data/Projects"; // TODO(Launcher)
}

auto launcher_project_info_folder(Project const& project) -> std::filesystem::path
{
    return path_to_launcher_project_info() / reg::to_string(project.uuid);
}

} // namespace Lab