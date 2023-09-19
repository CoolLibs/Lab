#include "utils.h"
#include <CommandCore/CommandExecutionContext_Ref.h>
#include <CommandLineArgs/CommandLineArgs.h>
#include <Common/Path.h>
#include <ProjectManager/Command_OpenProject.h>
#include <ProjectManager/internal_utils.h>
#include <filesystem>
#include "Command_NewProject.h"
#include "Command_OpenBackupProject.h"
#include "Command_OpenProject.h"
#include "Command_SaveProject.h"
#include "Command_SaveProjectAs.h"
#include "Cool/File/File.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/UserSettings/UserSettings.h"
#include "FileExtension.h"
#include "Project.h"
#include "RecentlyOpened.h"

namespace Lab {

void initial_project_opening(CommandExecutionContext_Ref const& ctx)
{
    auto const path = [&]() -> std::filesystem::path {
        // Load the project that was requested, e.g. when double-clicking on a .coollab file.
        if (!command_line_args().get().empty())
        {
            return command_line_args().get()[0];
        }
        // Try the untitled project.
        return Path::untitled_project();
    }();
    if (!std::filesystem::exists(path))
        return; // Avoid error message caused by the fact that the file doesn't exist. It is legit if the untitled project doesn't exist, we don't want an error in that case.

    ctx.execute(Command_OpenProject{
        .path = path,
    });
}

static auto project_dialog_args(CommandExecutionContext_Ref const& ctx) -> Cool::File::file_dialog_args
{
    auto initial_folder = ctx.recently_opened_projects().most_recent_path(); // If a project is currently open this will be its path, else it will be the path of the last open one.
    if (initial_folder)
        initial_folder = initial_folder->parent_path(); // Remove the file part, keep only the folder.
    else
        initial_folder = "";

    return Cool::File::file_dialog_args{
        .file_filters   = {{"Coollab project", COOLLAB_FILE_EXTENSION}},
        .initial_folder = *initial_folder,
    };
}

void dialog_to_open_project(CommandExecutionContext_Ref const& ctx)
{
    auto const path = Cool::File::file_opening_dialog(project_dialog_args(ctx));
    if (!path)
        return;

    ctx.execute(Command_OpenProject{
        .path = *path,
    });
}

void dialog_to_open_recent_project(RecentlyOpened& recently_opened)
{
    recently_opened.open_window();
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

    // We are on an untitled project, ask to save it.
    bool has_saved{false};
    while (true) // If the user cancels the save dialog, we want to ask again if they want to save or not. This will prevent closing the dialog by mistake and then losing your changes.
    {
        if (boxer::show("You have unsaved changes. Do you want to save them? They will be lost otherwise.\n(NB: Actually you can still recover them by using \"Load Backup\" in the \"File\" menu.)", "Unsaved project", boxer::Style::Warning, boxer::Buttons::YesNo)
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
    std::filesystem::remove(Path::untitled_project()); // Users either saved their untitled project, or accepted to lose their changes.
    if (!has_saved)
        internal_project::save_project_to(ctx, Path::backup_project());
}

void imgui_open_save_project(CommandExecutionContext_Ref const& ctx)
{
    if (ImGui::MenuItem("New", "Ctrl+N"))
        ctx.execute(Command_NewProject{});
    if (ImGui::MenuItem("Open", "Ctrl+O"))
        dialog_to_open_project(ctx);
    if (ImGui::MenuItem("Open Recent", "Ctrl+R"))
        dialog_to_open_recent_project(ctx.recently_opened_projects());
    if (ImGui::MenuItem("Open Backup", "Ctrl+Shift+R"))
        ctx.execute(Command_OpenBackupProject{});
    Cool::ImGuiExtras::help_marker("If you accidentally don't save your changes when a message box prompts you to do so before they get lost, you can actually recover them here.");
    if (ImGui::MenuItem("Save", "Ctrl+S"))
        ctx.execute(Command_SaveProject{});
    if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        dialog_to_save_project_as(ctx);
}

} // namespace Lab