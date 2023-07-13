#pragma once
#include <CommandCore/CommandExecutor_TopLevel_Ref.h>

namespace Lab {

/// Checks which project should be open, and opens it.
void initial_project_opening(CommandExecutor_TopLevel_Ref const&, std::filesystem::path const& project_path_from_previous_session);

/// Opens a file dialog and then opens the selected project. Does nothing if the file dialog is cancelled.
void dialog_to_open_project(CommandExecutor_TopLevel_Ref const&);

/// Opens a file dialog and then saves the project in the selected file. Does nothing if the file dialog is cancelled.
void dialog_to_save_project_as(CommandExecutor_TopLevel_Ref const&);

/// Saves the project in the current project path.
void save_current_project(CommandExecutor_TopLevel_Ref const&);

void imgui_open_save_project(CommandExecutor_TopLevel_Ref const&);

} // namespace Lab