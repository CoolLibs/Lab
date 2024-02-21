#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {

template<typename T>
struct ReversibleCommand_SetVariableDefaultMetadata;

template<typename T>
struct Command_SetVariableDefaultMetadata {
    Cool::SharedVariableStrongRef<T> var_ref{};
    Cool::VariableMetadata<T>        default_metadata{};

    void execute(CommandExecutionContext_Ref const&) const
    {
        var_ref.variable->default_metadata() = default_metadata;
    }

    auto to_string() const -> std::string
    {
        return "Set " + std::to_string(var_ref.id()) + "'s default metadata";
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariableDefaultMetadata<T>
    {
        return ReversibleCommand_SetVariableDefaultMetadata<T>{
            .fwd                  = *this,
            .old_default_metadata = var_ref.variable->default_metadata(),
        };
    }
};

template<typename T>
struct ReversibleCommand_SetVariableDefaultMetadata {
    Command_SetVariableDefaultMetadata<T> fwd{};
    Cool::VariableMetadata<T>             old_default_metadata{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        fwd.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref const&) const
    {
        fwd.var_ref.variable->default_metadata() = old_default_metadata;
    }

    auto to_string() const -> std::string
    {
        return fwd.to_string();
    }

    auto merge(ReversibleCommand_SetVariableDefaultMetadata<T> const&) const -> std::optional<ReversibleCommand_SetVariableDefaultMetadata<T>>
    {
        return std::nullopt;
    };
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetVariableDefaultMetadata<T>& command)
{
    archive(
        cereal::make_nvp("Variable ref", command.var_ref),
        cereal::make_nvp("Default metadata", command.default_metadata)
    );
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetVariableDefaultMetadata<T>& command)
{
    archive(
        cereal::make_nvp("Forward", command.fwd),
        cereal::make_nvp("Old default metadata", command.old_default_metadata)
    );
}

} // namespace cereal
