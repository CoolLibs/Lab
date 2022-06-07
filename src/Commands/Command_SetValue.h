#pragma once

#include <stringify/stringify.hpp>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/LAB_REGISTER_COMMAND.h"
#include "CommandCore/LAB_REGISTER_REVERSIBLE_COMMAND.h"

namespace Lab {

namespace internal {

template<typename T>
void set_value_default_impl(CommandExecutionContext_Ref& ctx, const reg::Id<T>& id, const T& value)
{
    ctx.registries().set(id, value);
    ctx.set_dirty(id);
}

} // namespace internal

template<typename T>
struct ReversibleCommand_SetValue;

template<typename T>
struct Command_SetValue {
    reg::Id<T> id{};
    T          value{};

    void execute(CommandExecutionContext_Ref& ctx) const
    {
        internal::set_value_default_impl(ctx, id, value);
    }

    auto to_string() const -> std::string
    {
        return "Set " + reg::to_string(id) + " to " + Cool::stringify(value);
    }

    auto make_reversible(const MakeReversibleCommandContext_Ref& ctx) const
        -> ReversibleCommand_SetValue<T>
    {
        return ReversibleCommand_SetValue<T>{
            .forward_command = *this,
            .old_value       = *ctx.registries.get().get(id), // If the id isn't found in the registry this will crash, but this is what we want because this should never happen: it is a mistake to try to create a command for a variable that doesn't exist
        };
    }
};

template<typename T>
struct ReversibleCommand_SetValue {
    Command_SetValue<T> forward_command{};
    T                   old_value{};

    void execute(CommandExecutionContext_Ref& ctx) const
    {
        forward_command.execute(ctx);
    }

    void revert(CommandExecutionContext_Ref& ctx) const
    {
        internal::set_value_default_impl(ctx, forward_command.id, old_value);
    }

    auto to_string() const -> std::string
    {
        return "Set " + reg::to_string(forward_command.id) + " from " + Cool::stringify(old_value) +
               " to " + Cool::stringify(forward_command.value);
    }

    auto merge(const ReversibleCommand_SetValue<T>& previous) const -> std::optional<ReversibleCommand_SetValue<T>>
    {
        if (previous.forward_command.id == forward_command.id) {
            return ReversibleCommand_SetValue<T>{
                .forward_command = forward_command,
                .old_value       = previous.old_value,
            };
        }
        else {
            return {};
        }
    };
};

} // namespace Lab

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::Command_SetValue<T>& command)
{
    archive(cereal::make_nvp("Id", command.id),
            cereal::make_nvp("Value", command.value));
}

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetValue<T>& command)
{
    archive(cereal::make_nvp("Id", command.forward_command.id),
            cereal::make_nvp("Value", command.forward_command.value),
            cereal::make_nvp("Old value", command.old_value));
}

} // namespace cereal

#include "generated/register_set_value_commands.inl"