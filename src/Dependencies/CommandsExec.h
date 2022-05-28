#pragma once

#include "CameraManager.h"
#include "Commands.h"
#include "History.h"
#include "Registries.h"

namespace Lab {

class SetVariableDirty {
public:
    SetVariableDirty()
    {
    }
    template<typename T>
    void operator()(reg::Id<T>)
    {
        // if (_module.get().depends_on(id)) {
        //     _module.get().set_dirty();
        // }
        // if (_module2.get().depends_on(id)) {
        //     _module2.get().set_dirty();
        // }
    }

private:
    // std::reference_wrapper<Module> _module;
    // std::reference_wrapper<Module> _module2;
};

// This is a class rather than a struct because we want to use methods to access the different members
// this will make it easier to track who is using what
// and since this class is a big thing that makes everything accessible to everyone, it is especially important to be able to track the use that is made of each member
class CommandExecutionContext {
public:
    auto history() const -> const History& { return _data.history; }
    auto registries() -> Registries& { return _data.registries; }
    auto camera_manager() -> CameraManager& { return _data.camera_manager; }
    template<typename T>
    void set_dirty(const reg::Id<T>& id)
    {
        _data.set_dirty(id);
    }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<const History>      history;
        std::reference_wrapper<Registries>         registries;
        std::reference_wrapper<Lab::CameraManager> camera_manager;
        SetVariableDirty                           set_dirty;
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
void set_value_default_impl(CommandExecutionContext ctx, const reg::Id<T>& id, const T& value)
{
    ctx.registries().set(id, value);
    ctx.set_dirty(id);
}

template<typename T>
void set_value(CommandExecutionContext ctx, const reg::Id<T>& id, const T& value)
{
    set_value_default_impl(ctx, id, value);
}

template<>
inline void set_value(CommandExecutionContext ctx, const reg::Id<Cool::Camera>& id, const Cool::Camera& value)
{
    set_value_default_impl(ctx, id, value);
    ctx.camera_manager().set_camera(value);
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

class CommandDispatcher {
public:
    CommandDispatcher(CommandExecutor executor, History& history, Registries& registries)
        : _executor{executor}, _history{history}, _registries{registries}
    {
    }

    void dispatch(const Command& command) const
    {
        const auto reversible = make_reversible_command({_registries}, command); // Must be before the execution of the command because we need to retrieve the state of the app before execution to create the reversible command
        _executor.execute(command);
        if (reversible) {
            _history.get().push(*reversible, ReversibleCommandMerger{});
        }
    }

private:
    mutable CommandExecutor            _executor;
    std::reference_wrapper<History>    _history;
    std::reference_wrapper<Registries> _registries;
};

} // namespace Lab