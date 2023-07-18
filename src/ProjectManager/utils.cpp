#include "utils.h"
#include <CommandLineArgs/CommandLineArgs.h>
#include <Common/Path.h>
#include <filesystem>
#include "Command_NewProject.h"
#include "Command_OpenProject.h"
#include "Command_SaveProject.h"
#include "Command_SaveProjectAs.h"
#include "Cool/File/File.h"
#include "Project.h"

namespace Lab {

void initial_project_opening(CommandExecutor const& command_executor)
{
    auto const path = [&]() -> std::filesystem::path {
        // Load the project that was requested, e.g. when double-clicking on a .clb file.
        if (!command_line_args().get().empty())
        {
            return command_line_args().get()[0];
        }
        return Path::backup_project();
        // // Load the project that was open during the previous session.
        // else // TODO(Project) if(project path is valid)
        // {
        //     return project_path_from_previous_session;
        // }
        // Fallback to the default project. // TODO(Project) This should probably be done by the OpenProject command itself.
        // else {
        //     load(default project);
        // }
    }();
    if (!std::filesystem::exists(path))
        return; // Avoid error message caused by the fact that the file doesn't exist. It is legit if the backup project doesn't exist, we don't want an error in that case.

    command_executor.execute(Command_OpenProject{
        .path = path,
        // .save_previous_project = false, //Commented out: Not needed with the current loading / restoring logic.
    });
}

void dialog_to_open_project(CommandExecutor const& command_executor)
{
    auto const path = Cool::File::file_opening_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, of if the latter is nullopt, use the most recent proejct's path. Or if there is none then leave it empty.
    if (path)
    {
        command_executor.execute(Command_OpenProject{
            .path = *path,
        });
    }
}

auto dialog_to_save_project_as(CommandExecutor const& command_executor) -> bool
{
    auto const path = Cool::File::file_saving_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.
    if (path)
    {
        command_executor.execute(Command_SaveProjectAs{
            .path = *path,
        });
        return true;
    }
    return false;
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

void imgui_open_save_project(CommandExecutor const& command_executor)
{
    if (ImGui::MenuItem("New", "Ctrl+N"))
        command_executor.execute(Command_NewProject{});
    if (ImGui::MenuItem("Open", "Ctrl+O"))
        dialog_to_open_project(command_executor);
    if (ImGui::MenuItem("Open Recent", "Ctrl+R"))
    {
        // TODO(Project)
    }
    if (ImGui::MenuItem("Save", "Ctrl+S"))
        command_executor.execute(Command_SaveProject{});
    if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        dialog_to_save_project_as(command_executor);
}

} // namespace Lab