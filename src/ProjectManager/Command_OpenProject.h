#pragma once
#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

struct Command_OpenProject {
    std::filesystem::path path{};
    // bool                  save_previous_project{true};

    void               execute(CommandExecutionContext_Ref const& ctx) const;
    [[nodiscard]] auto to_string() const -> std::string;
};

} // namespace Lab

namespace cereal {
template<class Archive>
void serialize(Archive&, Lab::Command_OpenProject&)
{
}
} // namespace cereal
