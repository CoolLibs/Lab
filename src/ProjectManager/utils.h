#pragma once
#include <CommandCore/CommandExecutionContext_Ref.h>
#include <CommandCore/CommandExecutor.h>

namespace Lab {

/// Checks which project should be open, and opens it.
void initial_project_opening(CommandExecutionContext_Ref const&);

/// Opens a file dialog and then opens the selected project. Does nothing if the file dialog is canceled.
void dialog_to_open_project(CommandExecutionContext_Ref const&);

/// Opens a file dialog and then saves the project in the selected file. Does nothing if the file dialog is canceled.
/// Returns false iff the dialog was canceled.
auto dialog_to_save_project_as(CommandExecutionContext_Ref const&) -> bool;

void before_project_destruction(CommandExecutionContext_Ref const&);

void imgui_open_save_project(CommandExecutionContext_Ref const&);

auto launcher_project_info_folder(Project const& project) -> std::filesystem::path;

} // namespace Lab