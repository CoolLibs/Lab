#pragma once

#include "History.h"

namespace Lab {

class Module;

class SetVariableDirty {
public:
    SetVariableDirty(Module& module, Module& module2)
        : _module{module}
        , _module2{module2}
    {
    }
    template<typename T>
    void operator()(reg::Id<T> id)
    {
        if (_module.get().depends_on(id)) {
            _module.get().set_dirty();
        }
        if (_module2.get().depends_on(id)) {
            _module2.get().set_dirty();
        }
    }

private:
    std::reference_wrapper<Module> _module;
    std::reference_wrapper<Module> _module2;
};

// This is a class rather than a struct because we want to use methods to access the different members
// this will make it easier to track who is using what
// and since this class is a big thing that makes everything accessible to everyone, it is especially important to be able to track the use that is made of each member
class CommandExecutionContext {
public:
    auto history() -> const History& { return _data.history; }
    auto registries() -> Registries& { return _data.registries; }
    template<typename T>
    void set_dirty(const reg::Id<T>& id)
    {
        _data.set_dirty(id);
    }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<const History> history;
        std::reference_wrapper<Registries>    registries;
        SetVariableDirty                      set_dirty;
    };
    explicit CommandExecutionContext(Data data)
        : _data{data} {}

private:
    Data _data;
};

inline void execute_command(CommandExecutionContext ctx, Command_FinishedEditingValue)
{
    ctx.history().dont_merge_next_command();
}

template<typename T>
void set_value(CommandExecutionContext ctx, const reg::Id<T>& id, const T& value)
{
    ctx.registries().set(id, value);
    ctx.set_dirty(id);
}

template<typename T>
void execute_command(CommandExecutionContext ctx, Command_SetValue<T> cmd)
{
    set_value(ctx, cmd.id, cmd.value);
}

template<typename T>
void execute_command(CommandExecutionContext ctx, ReversibleCommand_SetValue<T> cmd)
{
    set_value(ctx, cmd.id, cmd.value);
}

template<typename T>
void revert_command(CommandExecutionContext ctx, ReversibleCommand_SetValue<T> cmd)
{
    set_value(ctx, cmd.id, cmd.old_value);
}

class CommandExecutor {
public:
    explicit CommandExecutor(CommandExecutionContext context)
        : _context{context}
    {
    }

    void execute(const Command& command)
    {
        std::visit([&](auto&& cmd) { execute_command(_context, cmd); }, command);
    }

private:
    CommandExecutionContext _context;
};

class ReversibleCommandExecutor {
public:
    explicit ReversibleCommandExecutor(CommandExecutionContext context)
        : _context{context}
    {
    }

    void execute(const ReversibleCommand& command)
    {
        std::visit([&](auto&& cmd) { execute_command(_context, cmd); }, command);
    }

    void revert(const ReversibleCommand& command)
    {
        std::visit([&](auto&& cmd) { revert_command(_context, cmd); }, command);
    }

private:
    CommandExecutionContext _context;
};

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
    std::reference_wrapper<Registries> registries;
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

class CommandDispatcher {
public:
    CommandDispatcher(CommandExecutor executor, History& history, Registries& registries)
        : _executor{executor}, _history{history}, _registries{registries}
    {
    }

    void dispatch(const Command& command)
    {
        const auto reversible = make_reversible_command({_registries}, command); // Must be before the execution of the command because we need to retrieve the state of the app before execution to create the reversible command
        _executor.execute(command);
        if (reversible) {
            _history.get().push(*reversible, ReversibleCommandMerger{});
        }
    }

private:
    CommandExecutor                    _executor;
    std::reference_wrapper<History>    _history;
    std::reference_wrapper<Registries> _registries;
};

} // namespace Lab