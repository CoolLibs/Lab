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

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

} // namespace Lab