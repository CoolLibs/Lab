#pragma once
#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

struct Command_PackageProjectInto {
    std::filesystem::path folder_path{};
    bool                  register_project_in_the_launcher{};

    void               execute(CommandExecutionContext_Ref const& ctx) const;
    [[nodiscard]] auto to_string() const -> std::string;
};

} // namespace Lab

namespace ser20 {
template<class Archive>
void serialize(Archive&, Lab::Command_PackageProjectInto&)
{
}
} // namespace ser20
