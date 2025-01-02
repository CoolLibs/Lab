#pragma once
#include <CommandCore/CommandExecutionContext_Ref.h>

namespace Lab::internal_project {

void set_current_project_path(CommandExecutionContext_Ref const&, std::optional<std::filesystem::path>);
/// Returns true iff the save was successful.
auto save_project_to(CommandExecutionContext_Ref const&, std::filesystem::path const&) -> bool;
/// Returns true iff the save was successful.
auto package_project_into(CommandExecutionContext_Ref const&, std::filesystem::path const& folder_path) -> bool;
void error_when_save_failed(std::filesystem::path const&);

} // namespace Lab::internal_project