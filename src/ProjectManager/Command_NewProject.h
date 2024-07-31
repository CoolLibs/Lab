#pragma once
#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

struct Command_NewProject {
    void               execute(CommandExecutionContext_Ref const&) const;
    [[nodiscard]] auto to_string() const -> std::string;
};

} // namespace Lab

namespace ser20 {
template<class Archive>
void serialize(Archive&, Lab::Command_NewProject&)
{
}
} // namespace ser20
