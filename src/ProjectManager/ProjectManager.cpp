#include "ProjectManager.h"
#include <CommandLineArgs/CommandLineArgs.h>
#include <Project.h>
#include <ProjectManager/ProjectManager.h>
#include "Command_OpenProject.h"
#include "Command_SaveProject.h"
#include "Command_SaveProjectAs.h"
#include "Cool/File/File.h"

namespace Lab {

void ProjectManager::initial_project_opening(CommandExecutor_TopLevel_Ref const& command_executor)
{
    // Load the project that was requested, e.g. when double-clicking on a .clb file.
    if (!command_line_args().get().empty())
    {
        command_executor.execute(Command_OpenProject{
            .path = command_line_args().get()[0],
        });
        // load(project, command_line_args().get()[0]);
    }
    // Load the project that was open during the previous session.
    else // TODO(Project) if(project path is valid)
    {
        command_executor.execute(Command_OpenProject{
            .path = _project_path,
        });
    }
    // Fallback to the default project. // TODO(Project) This should probably be done by the OpenProject command itself.
    // else {
    //     load(default project);
    // }
}

void ProjectManager::open(CommandExecutor_TopLevel_Ref const& command_executor)
{
    auto const path = Cool::File::file_opening_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
    if (path)
    {
        command_executor.execute(Command_OpenProject{
            .path = *path,
        });
    }
}

void ProjectManager::save_as(CommandExecutor_TopLevel_Ref const& command_executor)
{
    auto const path = Cool::File::file_saving_dialog({.file_filters = {{"Coollab project", "clb"}}, .initial_folder = ""}); // TODO(Project) initial_folder should be the folder of _project_path, unless the latter is the path to the default coollab project. In which case leave initial_folder empty.        if (path)
    if (path)
    {
        command_executor.execute(Command_SaveProjectAs{
            .path = *path,
        });
    }
}

void ProjectManager::save(CommandExecutor_TopLevel_Ref const& command_executor) const
{
    command_executor.execute(Command_SaveProject{});
}

void ProjectManager::imgui(CommandExecutor_TopLevel_Ref const& command_executor)
{
    if (ImGui::MenuItem("Open", "Ctrl+O"))
        open(command_executor);
    if (ImGui::MenuItem("Save", "Ctrl+S"))
        save(command_executor);
    if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        save_as(command_executor);
}

} // namespace Lab