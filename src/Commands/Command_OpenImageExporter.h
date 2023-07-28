#pragma once

#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

class App;

struct Command_OpenImageExporter {
    void execute(CommandExecutionContext_Ref const& ctx) const;

    auto to_string() const -> std::string
    {
        return "Open Image Exporter Window";
    }
};

} // namespace Lab

namespace cereal {

template<class Archive>
void serialize(Archive&, Lab::Command_OpenImageExporter&)
{
}

} // namespace cereal