#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "CommandCore/var_ref_to_string.h"

namespace Lab {

template<typename T>
struct ReversibleCommand_SetVariableMetadata;

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
        return fmt::format("Set {}'s metadata", Lab::to_string(var_ref));
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariableMetadata<T>
    {
        return ReversibleCommand_SetVariableMetadata<T>{
            .fwd          = *this,
            .old_metadata = var_ref.variable->metadata(),
        };
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Variable ref", var_ref),
            cereal::make_nvp("Metadata", metadata)
        );
    }
};

template<typename T>
struct ReversibleCommand_SetVariableMetadata {
    Command_SetVariableMetadata<T> fwd{};
    Cool::VariableMetadata<T>      old_metadata{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        fwd.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref const&) const
    {
        fwd.var_ref.variable->metadata() = old_metadata;
    }

    auto to_string() const -> std::string
    {
        return fwd.to_string();
    }

    auto merge(ReversibleCommand_SetVariableMetadata<T> const&) const -> std::optional<ReversibleCommand_SetVariableMetadata<T>>
    {
        return std::nullopt;
    };

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Forward", fwd),
            cereal::make_nvp("Old metadata", old_metadata)
        );
    }
};

} // namespace Lab