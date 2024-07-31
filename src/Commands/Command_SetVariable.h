#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include <Cool/StrongTypes/Gradient.h>
#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "CommandCore/var_ref_to_string.h"

namespace Lab {

namespace internal {

template<typename T>
void set_value(Cool::SharedVariableStrongRef<T> const& var_ref, T const& value)
{
    if constexpr (std::is_same_v<T, Cool::Gradient>)
    {
        // Request shader code generation only if the number of marks has changed, or the wrap mode or interpolation mode has changed.
        if (var_ref.variable->value().value.gradient().get_marks().size() != value.value.gradient().get_marks().size()
            || var_ref.variable->value().wrap_mode != value.wrap_mode
            || var_ref.variable->value().value.gradient().interpolation_mode() != value.value.gradient().interpolation_mode())
        {
            var_ref.secondary_dirty_flag.set_dirty();
        }
        else
        {
            var_ref.dirty_flag.set_dirty();
        }
    }
    else
    {
        var_ref.dirty_flag.set_dirty();
    }
    var_ref.variable->value() = value;
}

} // namespace internal

template<typename T>
struct ReversibleCommand_SetVariable;

template<typename T>
struct Command_SetVariable {
    Cool::SharedVariableStrongRef<T> var_ref;
    T                                value{};

    void execute(CommandExecutionContext_Ref const&) const
    {
        internal::set_value(var_ref, value);
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {} to {}", Lab::to_string(var_ref), Cool::stringify(value));
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariable<T>
    {
        return ReversibleCommand_SetVariable<T>{
            .fwd       = *this,
            .old_value = var_ref.variable->value(),
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
            ser20::make_nvp("Value", value)
        );
    }
};

template<typename T>
struct ReversibleCommand_SetVariable {
    Command_SetVariable<T> fwd{};
    T                      old_value{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        fwd.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref const&) const
    {
        internal::set_value(fwd.var_ref, old_value);
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {} from {} to {}", Lab::to_string(fwd.var_ref), Cool::stringify(old_value), Cool::stringify(fwd.value));
    }

    auto merge(ReversibleCommand_SetVariable<T> const& previous) const -> std::optional<ReversibleCommand_SetVariable<T>>
    {
        if (previous.fwd.var_ref.id() != fwd.var_ref.id())
            return std::nullopt;

        return ReversibleCommand_SetVariable<T>{
            .fwd       = fwd,
            .old_value = previous.old_value,
        };
    };

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Forward", fwd),
            ser20::make_nvp("Old value", old_value)
        );
    }
};

} // namespace Lab