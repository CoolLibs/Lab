#pragma once

#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

class App;

struct Command_OpenVideoExporter {
    void execute(CommandExecutionContext_Ref& ctx) const;

    auto to_string() const -> std::string
    {
        return "Open Video Exporter Window";
    }
};

} // namespace Lab

namespace cereal {

template<class Archive>
void serialize(Archive&, Lab::Command_OpenVideoExporter&)
{
}

} // namespace cereal

LAB_REGISTER_COMMAND(Lab::Command_OpenVideoExporter)