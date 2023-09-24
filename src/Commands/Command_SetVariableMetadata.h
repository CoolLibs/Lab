#pragma once

#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

template<typename T>
struct Command_SetVariableMetadata {
    Cool::VariableId<T>       id{};
    Cool::VariableMetadata<T> metadata{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        ctx.registries().with_mutable_ref<Cool::Variable<T>>(id, [&](Cool::Variable<T>& variable) {
            variable.metadata() = metadata;
        });
    }

    auto to_string() const -> std::string
    {
        return "Set " + reg::to_string(id) + "'s metadata";
    }
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetVariableMetadata<T>& command)
{
    archive(
        cereal::make_nvp("Id", command.id),
        cereal::make_nvp("Metadata", command.metadata)
    );
}

} // namespace cereal
