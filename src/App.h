#pragma once

#include <Cool/Default/DefaultApp.h>
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

private:
    Registries              _registries; // First because modules need the registries when they get created
    ShaderManagerManager    _shader_manager;
    std::unique_ptr<Module> _current_module;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Shader Manager Manager", _shader_manager),
                cereal::make_nvp("Default App", *reinterpret_cast<DefaultApp*>(this)));
    }
#if !IS0_TEST_NODES
    // Must be declared last because its constructor modifies App, and its destructor requires all other members to still be alive
    Cool::AutoSerializer<App> _auto_serializer{Cool::Path::root() + "/last-session-cache.json", "App", *this};
#endif
};

} // namespace Lab