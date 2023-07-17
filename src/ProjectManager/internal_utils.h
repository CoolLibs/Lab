#pragma once
#include <CommandCore/CommandExecutionContext_Ref.h>

namespace Lab {

void set_current_project_path(CommandExecutionContext_Ref const&, std::optional<std::filesystem::path> const&);
void set_current_project(CommandExecutionContext_Ref const&, Project&&, std::optional<std::filesystem::path> const& project_path);
void save_project_to(CommandExecutionContext_Ref const&, std::filesystem::path const&);

} // namespace Lab