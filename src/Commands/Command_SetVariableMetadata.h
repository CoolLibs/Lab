#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

template<typename T>
struct Command_SetVariableMetadata {
    Cool::SharedVariableStrongRef<T> var_ref{};
    Cool::VariableMetadata<T>        metadata{};

    void execute(CommandExecutionContext_Ref const&) const
    {
        var_ref.variable->metadata() = metadata;
    }

    auto to_string() const -> std::string
    {
        return "Set " + std::to_string(var_ref.id()) + "'s metadata";
    }
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetVariableMetadata<T>& command)
{
    archive(
        cereal::make_nvp("Variable ref", command.var_ref),
        cereal::make_nvp("Metadata", command.metadata)
    );
}

} // namespace cereal
