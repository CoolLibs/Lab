#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "CommandCore/var_ref_to_string.h"

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
        return fmt::format("Set {}'s default metadata", Lab::to_string(var_ref));
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariableDefaultMetadata<T>
    {
        return ReversibleCommand_SetVariableDefaultMetadata<T>{
            .fwd                  = *this,
            .old_default_metadata = var_ref.variable->default_metadata(),
        };
    }

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Variable ref", var_ref),
            ser20::make_nvp("Default metadata", default_metadata)
        );
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

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Forward", fwd),
            ser20::make_nvp("Old default metadata", old_default_metadata)
        );
    }
};

} // namespace Lab