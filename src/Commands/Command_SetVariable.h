#pragma once
#include <Cool/Dependencies/SharedVariable.h>
#include <Cool/StrongTypes/Gradient.h>
#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

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
        return fmt::format("Set {} to {}", var_ref.id(), Cool::stringify(value));
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariable<T>
    {
        return ReversibleCommand_SetVariable<T>{
            .forward_command = *this,
            .old_value       = var_ref.variable->value(),
        };
    }
};

template<typename T>
struct ReversibleCommand_SetVariable {
    Command_SetVariable<T> forward_command{};
    T                      old_value{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        forward_command.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref const&) const
    {
        internal::set_value(forward_command.var_ref, old_value);
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {} from {} to {}", forward_command.var_ref.id(), Cool::stringify(old_value), Cool::stringify(forward_command.value));
    }

    auto merge(ReversibleCommand_SetVariable<T> const& previous) const -> std::optional<ReversibleCommand_SetVariable<T>>
    {
        if (previous.forward_command.var_ref.id() != forward_command.var_ref.id())
            return std::nullopt;

        return ReversibleCommand_SetVariable<T>{
            .forward_command = forward_command,
            .old_value       = previous.old_value,
        };
    };
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetVariable<T>& command)
{
    archive(
        cereal::make_nvp("Variable ref", command.var_ref),
        cereal::make_nvp("Value", command.value)
    );
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetVariable<T>& command)
{
    archive(
        cereal::make_nvp("Forward", command.forward_command),
        cereal::make_nvp("Old value", command.old_value)
    );
}

} // namespace cereal
