#pragma once
#include <CommandCore/CommandExecutionContext_Ref.h>

namespace Lab {

void set_current_project_path(CommandExecutionContext_Ref const&, std::optional<std::filesystem::path> const&);
void save_project_to(CommandExecutionContext_Ref const&, std::filesystem::path const&);

} // namespace Lab