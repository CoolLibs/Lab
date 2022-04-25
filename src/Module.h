#pragma once
#include <cereal/types/polymorphic.hpp>
#include <cmd/cmd.hpp>
#include <glm/glm.hpp>
#include "Commands.h"
#include "Dependencies.h"
#include "Registries.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// The ui() method should be const, because it sould only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level)
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

class Ui;

class Module {
public:
    virtual ~Module() = default;
    void do_rendering(const Registries& registries)
    {
        render(registries);
        _is_dirty = false;
    }
    virtual void imgui_windows(Ui ui) = 0;
    virtual void update(){};

    auto needs_rendering() const -> bool { return _is_dirty || force_rendering(); };

    auto depends_on(reg::AnyId) const -> bool
    {
        return true;
    }

    void set_dirty()
    {
        _is_dirty = true;
    }

protected:
    virtual void render(const Registries& registries) = 0;
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
    SetDirty(Module& module)
        : _module{module}
    {
    }
    template<typename T>
    void operator()(reg::Id<T> id)
    {
        if (_module.get().depends_on(id)) {
            _module.get().set_dirty();
        }
    }

private:
    std::reference_wrapper<Module> _module;
};

template<typename T>
void execute_command(Registries& registries, SetDirty& set_dirty, ReversibleCommand_SetValue<T> cmd)
{
    registries.set(cmd.id, cmd.value);
    set_dirty(cmd.id);
}

template<typename T>
void revert_command(Registries& registries, SetDirty& set_dirty, ReversibleCommand_SetValue<T> cmd)
{
    registries.set(cmd.id, cmd.old_value);
    set_dirty(cmd.id);
}

class ReversibleCommandExecutor {
public:
    ReversibleCommandExecutor(Registries& registries, SetDirty set_dirty)
        : _registries{registries}, _set_dirty{set_dirty}
    {
    }

    void execute(const ReversibleCommand& command)
    {
        std::visit([&](auto cmd) { execute_command(_registries, _set_dirty, cmd); }, command);
    }

    void revert(const ReversibleCommand& command)
    {
        std::visit([&](auto cmd) { revert_command(_registries, _set_dirty, cmd); }, command);
    }

private:
    std::reference_wrapper<Registries> _registries;
    SetDirty                           _set_dirty;
};

class ReversibleCommandDispatcher {
public:
    ReversibleCommandDispatcher(ReversibleCommandExecutor executor, cmd::History<ReversibleCommand>& history)
        : _executor{executor}, _history{history}
    {
    }

    void dispatch(const ReversibleCommand& command)
    {
        _executor.execute(command);
        _history.get().push(command);
    }

private:
    ReversibleCommandExecutor                               _executor;
    std::reference_wrapper<cmd::History<ReversibleCommand>> _history;
};

class Ui {
public:
    Ui(const Registries& registries, Lab::ReversibleCommandDispatcher commands)
        : _registries{registries}, _commands{commands} {}

    struct WindowParams {
        std::string name;
    };

    template<typename UiFunction>
    static void window(const WindowParams& params, UiFunction&& ui_function)
    {
        ImGui::Begin(params.name.c_str());
        ui_function();
        ImGui::End();
    }

    void widget(const char* name, reg::Id<glm::vec3> colorId)
    {
        const auto color = _registries.get().get(colorId);
        if (color) {
            auto color_value = *color;
            if (ImGui::ColorEdit3(name, glm::value_ptr(color_value))) {
                _commands.dispatch(ReversibleCommand_SetValue{.id        = colorId,
                                                              .value     = color_value,
                                                              .old_value = *color});
            }
        }
    }

private:
    std::reference_wrapper<const Registries> _registries;
    ReversibleCommandDispatcher              _commands;
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
        return "[value_to_string] ERROR: Couldn't find a stringification function for this type" + typeid(T).name();
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