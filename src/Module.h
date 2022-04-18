#pragma once
#include <glm/glm.hpp>
#include "Commands.h"
#include "Dependencies.h"
#include "Registries.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

class Ui;

class Module {
public:
    virtual ~Module() = default;
    void do_rendering(const Registries& registries)
    {
        render(registries);
        _is_dirty = false;
    }
    virtual void imgui_windows(Ui& ui) = 0;
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
        if (_module.depends_on(id)) {
            _module.set_dirty();
        }
    }

private:
    Module& _module;
};

template<typename T>
inline void handle_command(Registries& registries, SetDirty& set_dirty, ReversibleCommand_SetValue<T> cmd)
{
    registries.set(cmd.id, cmd.value);
    set_dirty(cmd.id);
}

class ReversibleCommandDispatcher {
public:
    ReversibleCommandDispatcher(Registries& registries, SetDirty& set_dirty)
        : _registries{registries}, _set_dirty{set_dirty}
    {
    }

    void dispatch(const ReversibleCommand& command)
    {
        std::visit([&](auto cmd) { handle_command(_registries, _set_dirty, cmd); }, command);
    }

private:
    Registries& _registries;
    SetDirty&   _set_dirty;
    //History& _history;
};

class Ui {
public:
    Ui(const Registries& registries, Lab::ReversibleCommandDispatcher& commands)
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
        const auto color = _registries.get(colorId);
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
    const Registries&            _registries;
    ReversibleCommandDispatcher& _commands;
};

} // namespace Lab