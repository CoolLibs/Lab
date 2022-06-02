#pragma once

#include "Commands/Commands.h"
#include "Dependencies/CommandLogger.h"
#include "History.h"
#include "InputSlot.h"
#include "VariableRegistries.h"

namespace Lab {

using AllInputSlots = std::vector<AnyInputSlotRefToConst>;

class SetVariableDirty {
public:
    SetVariableDirty(AllInputSlots slots, SetDirtyFlag set_dirty)
        : _slots{slots}
        , _set_dirty{set_dirty}
    {
    }

    template<typename T>
    void operator()(reg::Id<T> variable_id)
    {
        for (const auto& slot : _slots) {
            const auto* slot_of_right_type = std::get_if<std::reference_wrapper<const Lab::InputSlot<T>>>(&slot);
            if (slot_of_right_type) {
                slot_of_right_type->get().set_dirty_if_depends_on(variable_id, _set_dirty);
            }
        }
    }

private:
    AllInputSlots _slots;
    SetDirtyFlag  _set_dirty;
};

// This is a class rather than a struct because we want to use methods to access the different members
// this will make it easier to track who is using what
// and since this class is a big thing that makes everything accessible to everyone, it is especially important to be able to track the use that is made of each member
class CommandExecutionContext {
public:
    auto history() const -> const History& { return _data.history; }
    auto registries() -> VariableRegistries& { return _data.registries; }
    template<typename T>
    void set_dirty(const reg::Id<T>& id)
    {
        _data.set_dirty(id);
    }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<const History>      history;
        std::reference_wrapper<VariableRegistries> registries;
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
    auto name = reg::to_string(id);
    ctx.registries().set(id, value);
    ctx.set_dirty(id);
}

template<typename T>
void set_value(CommandExecutionContext ctx, const reg::Id<T>& id, const T& value)
{
    set_value_default_impl(ctx, id, value);
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
    CommandDispatcher(CommandExecutor     executor,
                      History&            history,
                      VariableRegistries& registries,
                      CommandLogger&      command_logger)
        : _executor{executor}
        , _history{history}
        , _variable_registries{registries}
        , _command_logger{command_logger}
    {
    }

    void dispatch(const Command& command) const
    {
        _command_logger.get().push(command);
        const auto reversible = make_reversible_command({_variable_registries}, command); // Must be before the execution of the command because we need to retrieve the state of the app before execution to create the reversible command
        _executor.execute(command);
        if (reversible) {
            _history.get().push(*reversible, ReversibleCommandMerger{});
        }
    }

private:
    mutable CommandExecutor                    _executor;
    std::reference_wrapper<History>            _history;
    std::reference_wrapper<VariableRegistries> _variable_registries;
    std::reference_wrapper<CommandLogger>      _command_logger;
};

} // namespace Lab