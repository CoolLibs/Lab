#pragma once

#include <Cool/Default/DefaultApp.h>
#include <cmd/cereal.hpp>
#include <cmd/cmd.hpp>
#include <reg/cereal.hpp>
#include "Module.h"
#include "Registries.h"
#include "ShaderManager/ShaderManagerManager.h"

namespace Lab {

class App : public CoolDefault::DefaultApp {
public:
    explicit App(Cool::WindowManager& windows);

    void update() override;

    void imgui_windows() override;
    void imgui_menus() override;

    void on_keyboard_event(const Cool::KeyboardEvent& event) override;
    void on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event) override;

private:
    void render(Cool::RenderTarget& render_target, float time);

    auto set_dirty() { return SetDirty{*_current_module}; }
    auto commands_executor() { return ReversibleCommandExecutor{_registries, set_dirty()}; }
    auto commands_dispatcher() { return ReversibleCommandDispatcher{commands_executor(), _history}; }
    auto ui() { return Ui{_registries, commands_dispatcher()}; }

private:
    Registries                      _registries; // First because modules need the registries when they get created
    reg::Id<int>                    _intId;
    ShaderManagerManager            _shader_manager;
    std::unique_ptr<Module>         _current_module;
    cmd::History<ReversibleCommand> _history;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Shader Manager Manager", _shader_manager),
                cereal::make_nvp("Default App", *reinterpret_cast<DefaultApp*>(this)),
                cereal::make_nvp("Registries", _registries),
                cereal::make_nvp("Int ID", _intId),
                cereal::make_nvp("History", _history),
                cereal::make_nvp("Module", _current_module));
    }
#if !IS0_TEST_NODES
    // Must be declared last because its constructor modifies App, and its destructor requires all other members to still be alive
    Cool::AutoSerializer<App> _auto_serializer{Cool::Path::root() + "/last-session-cache.json", "App", *this};
#endif
};

} // namespace Lab