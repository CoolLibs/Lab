#pragma once
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>
#include "Commands.h"
#include "Dependencies.h"
#include "History.h"
#include "InputSlot.h"
#include "Registries.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// The ui() method should be const, because it sould only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level)
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class Ui;

class InputProvider {
public:
    explicit InputProvider(const Registries& registries)
        : _registries{registries}
    {
    }

    glm::vec3 operator()(const InputSlot<glm::vec3>& slot)
    {
        const auto maybe_color = _registries.get().get(slot.id);
        return maybe_color.value_or(glm::vec3{});
    }

private:
    std::reference_wrapper<const Registries> _registries;
};

class Module {
public:
    virtual ~Module() = default;
    void do_rendering(InputProvider provider)
    {
        render(provider);
        _is_dirty = false;
    }
    virtual void imgui_windows(Ui ui) = 0;
    virtual void update(){};

    auto needs_rendering() const -> bool { return _is_dirty || force_rendering(); };

    virtual auto depends_on(reg::AnyId) const -> bool = 0;

    void set_dirty()
    {
        _is_dirty = true;
    }

protected:
    virtual void render(InputProvider provider) = 0;
    virtual auto force_rendering() const -> bool { return false; }
    // template<typename T>
    // T get();

private:
    // Dependencies _dependencies;
    bool _is_dirty = true;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // archive();
    }
};

class SetDirty {
public:
    SetDirty(Module& module, Module& module2)
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
        SetDirty                              set_dirty;
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

class Ui {
public:
    Ui(Registries& registries, CommandDispatcher commands)
        : _registries{registries}, _commands{commands} {}

    struct WindowParams {
        std::string_view name;
    };

    template<typename UiFunction>
    static void window(const WindowParams& params, UiFunction&& ui_function)
    {
        ImGui::Begin(params.name.data());
        ui_function();
        ImGui::End();
    }

    void widget(const char* name, reg::Id<glm::vec3> color_id, glm::vec3 current_value)
    {
        if (ImGui::ColorEdit3(name, glm::value_ptr(current_value))) {
            _commands.dispatch(Command_SetValue<glm::vec3>{.id    = color_id,
                                                           .value = current_value});
        }
        if (ImGui::IsItemDeactivatedAfterEdit()) {
            _commands.dispatch(Command_FinishedEditingValue{});
        }
    }

    void widget(const char* name, reg::Id<glm::vec3> color_id)
    {
        ImGui::PushID(this);
        auto color = _registries.get().get(color_id);
        if (color) {
            widget(name, color_id, *color);
        }
        ImGui::PopID();
    }

    void widget(const char* name, InputSlot<glm::vec3>& color_slot)
    {
        ImGui::PushID(this);
        auto color = _registries.get().get(color_slot.id);
        if (!color) {
            color         = glm::vec3{};
            color_slot.id = _registries.get().create(*color);
        }
        widget(name, color_slot.id, *color);
        ImGui::PopID();
    }

private:
    std::reference_wrapper<Registries> _registries;
    CommandDispatcher                  _commands;
};

template<typename T>
auto value_to_string(const T& value) -> std::string
{
    if constexpr (requires { to_string(value); }) {
        return to_string(value);
    }
    else if constexpr (requires { std::to_string(value); }) {
        return std::to_string(value);
    }
    else {
        return std::string{"[value_to_string] ERROR: Couldn't find a stringification function for this type: "} + typeid(T).name();
    }
}

template<typename T>
void imgui_show(const T& value)
{
    ImGui::Text("%s", value_to_string(value).c_str());
}

template<typename T>
inline void imgui_show(const reg::Id<T>& id)
{
    ImGui::Text("%s", reg::to_string(id).c_str());
}

template<typename T>
void imgui_show(const reg::Registry<T>& registry)
{
    std::shared_lock       lock{registry.mutex()};
    static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("table1", 2, flags)) {
        for (const auto& [id, value] : registry) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            imgui_show(value);
            ImGui::TableSetColumnIndex(1);
            imgui_show(id);
        }
        ImGui::EndTable();
    }
}

} // namespace Lab