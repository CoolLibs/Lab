#pragma once
#include <Cool/Dependencies/Input.h>
#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {

template<typename T>
struct ReversibleCommand_SetVariableDefaultValue;

template<typename T>
struct Command_SetVariableDefaultValue {
    Cool::InputStrongRef<T> input;
    T                       default_value{};

    void execute(CommandExecutionContext_Ref const&) const
    {
        input.variable->default_value() = default_value;
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {}'s default value to {}", input.id(), Cool::stringify(default_value));
    }

    auto make_reversible(MakeReversibleCommandContext_Ref const&) const
        -> ReversibleCommand_SetVariableDefaultValue<T>
    {
        return ReversibleCommand_SetVariableDefaultValue<T>{
            .forward_command   = *this,
            .old_default_value = input.variable->default_value(),
        };
    }
};

template<typename T>
struct ReversibleCommand_SetVariableDefaultValue {
    Command_SetVariableDefaultValue<T> forward_command{};
    T                                  old_default_value{};

    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        forward_command.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref const&) const
    {
        forward_command.input.variable->default_value() = old_default_value;
    }

    auto to_string() const -> std::string
    {
        return fmt::format("Set {}'s default value from {} to {}", forward_command.input.id(), Cool::stringify(old_default_value), Cool::stringify(forward_command.default_value));
    }

    auto merge(ReversibleCommand_SetVariableDefaultValue<T> const& previous) const -> std::optional<ReversibleCommand_SetVariableDefaultValue<T>>
    {
        return std::nullopt;
    };
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetVariableDefaultValue<T>& command)
{
    archive(
        cereal::make_nvp("Input", command.input),
        cereal::make_nvp("Default value", command.default_value)
    );
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetVariableDefaultValue<T>& command)
{
    archive(
        cereal::make_nvp("Forward", command.forward_command),
        cereal::make_nvp("Old default value", command.old_default_value)
    );
}

} // namespace cereal
