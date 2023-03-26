#pragma once

#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

template<typename T>
struct Command_SetVariableMetadata {
    Cool::VariableId<T>       id{};
    Cool::VariableMetadata<T> metadata{};

    void execute(CommandExecutionContext_Ref& ctx) const
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
#if COOL_SERIALIZATION
    archive(
        cereal::make_nvp("Id", command.id),
        cereal::make_nvp("Metadata", command.metadata)
    );
#else
    (void)archive;
#endif
}

} // namespace cereal

#include "generated/register_set_variable_metadata_commands.inl"