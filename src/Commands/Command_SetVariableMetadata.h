#pragma once
#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

template<typename T>
struct Command_SetVariableMetadata {
    Cool::Input<T>            input{};
    Cool::VariableMetadata<T> metadata{};

    void execute(CommandExecutionContext_Ref const&) const
    {
        input._variable->metadata() = metadata;
    }

    auto to_string() const -> std::string
    {
        return "Set " + std::to_string(input.id()) + "'s metadata";
    }
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetVariableMetadata<T>& command)
{
    archive(
        cereal::make_nvp("Input", command.input),
        cereal::make_nvp("Metadata", command.metadata)
    );
}

} // namespace cereal
