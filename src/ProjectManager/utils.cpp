#include "utils.h"
#include <CommandLineArgs/CommandLineArgs.h>
#include "Command_OpenProject.h"
#include "Command_SaveProject.h"
#include "Command_SaveProjectAs.h"
#include "Cool/File/File.h"

namespace Lab {

void initial_project_opening(CommandExecutor_TopLevel_Ref const& command_executor, std::filesystem::path const& project_path_from_previous_session)
{
    auto const path = [&]() -> std::filesystem::path {
        // Load the project that was requested, e.g. when double-clicking on a .clb file.
        if (!command_line_args().get().empty())
        {
            return command_line_args().get()[0];
        }
        // Load the project that was open during the previous session.
        else // TODO(Project) if(project path is valid)
        {
            return project_path_from_previous_session;
        }
        // Fallback to the default project. // TODO(Project) This should probably be done by the OpenProject command itself.
        // else {
        //     load(default project);
        // }
    }();
    command_executor.execute(Command_OpenProject{
        .path                  = path,
        .save_previous_project = false,
    });
}

void dialog_to_open_project(CommandExecutor_TopLevel_Ref const& command_executor)
{
    auto const path = Cool::File::file_opening_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
    if (path)
    {
        command_executor.execute(Command_OpenProject{
            .path = *path,
        });
    }
}

void dialog_to_save_project_as(CommandExecutor_TopLevel_Ref const& command_executor)
{
    auto const path = Cool::File::file_saving_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
    if (path)
    {
        command_executor.execute(Command_SaveProjectAs{
            .path = *path,
        });
    }
}

void save_current_project(CommandExecutor_TopLevel_Ref const& command_executor)
{
    command_executor.execute(Command_SaveProject{});
}

void imgui_open_save_project(CommandExecutor_TopLevel_Ref const& command_executor)
{
    if (ImGui::MenuItem("Open", "Ctrl+O"))
        dialog_to_open_project(command_executor);
    if (ImGui::MenuItem("Save", "Ctrl+S"))
        save_current_project(command_executor);
    if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        dialog_to_save_project_as(command_executor);
}

} // namespace Lab