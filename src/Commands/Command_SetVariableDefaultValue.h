#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "CommandCore/var_ref_to_string.h"

namespace Lab {

template<typename T>
struct ReversibleCommand_SetVariableDefaultValue;

template<typename T>
struct Command_SetVariableDefaultValue {
    Cool::SharedVariableStrongRef<T> var_ref{};
    T                                default_value{};

    void execute(CommandExecutionContext_Ref const&) const
    {
        var_ref.variable->default_value() = default_value;
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {}'s default value to {}", Lab::to_string(var_ref), Cool::stringify(default_value));
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariableDefaultValue<T>
    {
        return ReversibleCommand_SetVariableDefaultValue<T>{
            .fwd               = *this,
            .old_default_value = var_ref.variable->default_value(),
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
            ser20::make_nvp("Default value", default_value)
        );
    }
};

template<typename T>
struct ReversibleCommand_SetVariableDefaultValue {
    Command_SetVariableDefaultValue<T> fwd{};
    T                                  old_default_value{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        fwd.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref const&) const
    {
        fwd.var_ref.variable->default_value() = old_default_value;
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {}'s default value from {} to {}", Lab::to_string(fwd.var_ref), Cool::stringify(old_default_value), Cool::stringify(fwd.default_value));
    }

    auto merge(ReversibleCommand_SetVariableDefaultValue<T> const&) const -> std::optional<ReversibleCommand_SetVariableDefaultValue<T>>
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
            ser20::make_nvp("Old default value", old_default_value)
        );
    }
};

} // namespace Lab