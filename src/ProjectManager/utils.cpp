#include "utils.h"
#include <CommandCore/CommandExecutionContext_Ref.h>
#include <CommandLineArgs/CommandLineArgs.h>
#include <Common/Path.h>
#include <filesystem>
#include "Command_NewProject.h"
#include "Command_OpenProject.h"
#include "Command_SaveProject.h"
#include "Command_SaveProjectAs.h"
#include "Cool/File/File.h"
#include "Cool/UserSettings/UserSettings.h"
#include "Project.h"
#include "RecentlyOpened.h"

namespace Lab {

void initial_project_opening(CommandExecutionContext_Ref const& ctx)
{
    auto const path = [&]() -> std::filesystem::path {
        // Load the project that was requested, e.g. when double-clicking on a .clb file.
        if (!command_line_args().get().empty())
        {
            return command_line_args().get()[0];
        }
        if (Cool::user_settings().open_most_recent_project_when_opening_coollab)
        {
            auto const path = ctx.recently_opened_projects().most_recent_path();
            if (path)
                return *path;
        }
        // Try the backup project. If it exists it means that the app did not exit successfully and there is a need to restore something.
        return Path::backup_project();
    }();
    if (!std::filesystem::exists(path))
        return; // Avoid error message caused by the fact that the file doesn't exist. It is legit if the backup project doesn't exist, we don't want an error in that case.

    ctx.execute(Command_OpenProject{
        .path = path,
    });
}

void dialog_to_open_project(CommandExecutor const& command_executor)
{
    auto const path = Cool::File::file_opening_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, of if the latter is nullopt, use the most recent proejct's path. Or if there is none then leave it empty.
    if (!path)
        return;

    command_executor.execute(Command_OpenProject{
        .path = *path,
    });
}

void dialog_to_open_recent_project(RecentlyOpened& recently_opened)
{
    recently_opened.open_window();
}

auto dialog_to_save_project_as(CommandExecutor const& command_executor) -> bool
{
    auto const path = Cool::File::file_saving_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.
    if (!path)
        return false;

    command_executor.execute(Command_SaveProjectAs{
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

void imgui_open_save_project(CommandExecutionContext_Ref const& ctx)
{
    if (ImGui::MenuItem("New", "Ctrl+N"))
        ctx.execute(Command_NewProject{});
    if (ImGui::MenuItem("Open", "Ctrl+O"))
        dialog_to_open_project(ctx.command_executor());
    if (ImGui::MenuItem("Open Recent", "Ctrl+R"))
        dialog_to_open_recent_project(ctx.recently_opened_projects());
    if (ImGui::MenuItem("Save", "Ctrl+S"))
        ctx.execute(Command_SaveProject{});
    if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        dialog_to_save_project_as(ctx.command_executor());
}

} // namespace Lab