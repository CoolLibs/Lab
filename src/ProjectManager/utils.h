#pragma once
#include <CommandCore/CommandExecutionContext_Ref.h>
#include <CommandCore/CommandExecutor.h>

namespace Lab {

/// Checks which project should be open, and opens it.
void initial_project_opening(CommandExecutor const&);

/// Opens a file dialog and then opens the selected project. Does nothing if the file dialog is cancelled.
void dialog_to_open_project(CommandExecutor const&);

/// Opens a file dialog and then saves the project in the selected file. Does nothing if the file dialog is cancelled.
/// Returns false iff the dialog was cancelled.
auto dialog_to_save_project_as(CommandExecutor const&) -> bool;

void before_project_destruction(CommandExecutionContext_Ref const&);

void imgui_open_save_project(CommandExecutor const&);

} // namespace Lab