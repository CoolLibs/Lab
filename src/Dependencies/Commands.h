#pragma once
#include <Cool/Camera/Camera.h>
#include <reg/reg.hpp>
#include "AllVariableTypes.h"
#include "VariableRegistries.h"

namespace Lab {

template<typename T>
struct ReversibleCommand_SetValue {
    reg::Id<T> id;
    T          value;
    T          old_value;
};

template<typename T>
struct Command_SetValue {
    reg::Id<T> id;
    T          value;
};

/// Submit this when for example you finished dragging a slider (e.g. when ImGui::IsItemDeactivatedAfterEdit() is true)
/// It will make sure that the commit will be locked in the history and future commands modifying the same value
/// won't be merged with the ones that were issued before Command_FinishedEditingValue
struct Command_FinishedEditingValue {
};

using Command =
    AllVariableTypes::
        wrap<Command_SetValue>::
            append<Command_FinishedEditingValue>::
                to<std::variant>;

using ReversibleCommand =
    AllVariableTypes::
        wrap<ReversibleCommand_SetValue>::
            to<std::variant>;

template<typename T>
auto merge_commands(const ReversibleCommand_SetValue<T>& prev, const ReversibleCommand_SetValue<T>& curr)
    -> std::optional<ReversibleCommand>
{
    if (prev.id == curr.id) {
        return ReversibleCommand_SetValue<T>{
            .id        = curr.id,
            .value     = curr.value,
            .old_value = prev.old_value,
        };
    }
    else {
        return std::nullopt;
    }
}

// Fallback if we don't find a function to merge the commands
template<typename T, typename U>
auto merge_commands(const T&, const U&) -> std::optional<ReversibleCommand>
{
    return std::nullopt;
}

class ReversibleCommandMerger {
public:
    static auto merge(const ReversibleCommand& prev, const ReversibleCommand& curr) -> std::optional<ReversibleCommand>
    {
        return std::visit([](const auto& p, const auto& c) { return merge_commands(p, c); },
                          prev, curr);
    }
};

struct MakeReversibleCommandContext {
    std::reference_wrapper<VariableRegistries> registries;
};

template<typename T>
auto make_reversible_command_impl(MakeReversibleCommandContext ctx, const Command_SetValue<T>& cmd) -> std::optional<ReversibleCommand>
{
    const auto old_value = ctx.registries.get().get(cmd.id);
    if (old_value) {
        return ReversibleCommand_SetValue<T>{
            .id        = cmd.id,
            .value     = cmd.value,
            .old_value = *old_value,
        };
    }
    else {
        Cool::Log::error("[make_reversible_command_impl] Trying to create a command for an id that isn't valid; I don't think this should happen.");
        return std::nullopt;
    }
}

// Fallback if we don't find a function to make a reversible command
template<typename T>
auto make_reversible_command_impl(MakeReversibleCommandContext, const T&) -> std::optional<ReversibleCommand>
{
    return std::nullopt;
}

inline auto make_reversible_command(MakeReversibleCommandContext ctx, const Command& command) -> std::optional<ReversibleCommand>
{
    return std::visit([ctx](auto&& cmd) { return make_reversible_command_impl(ctx, cmd); }, command);
}

} // namespace Lab

#include <cereal/types/variant.hpp>

namespace cereal {

template<class Archive, typename T>
void serialize(Archive& archive, Lab::ReversibleCommand_SetValue<T>& command)
{
    archive(command.id,
            command.value,
            command.old_value);
}

} // namespace cereal