#pragma once

#include <Cool/Default/DefaultApp.h>
#include "ShaderManager/ShaderManagerManager.h"

using namespace Cool;

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
    ShaderManagerManager _shader_manager;
    // Must be declared last because its constructor modifies App, and its destructor requires all other members to still be alive
    Cool::AutoSerializer<App> _auto_serializer{Cool::File::root_dir() + "/last-session-cache.json", "App", *this};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Shader Manager Manager", _shader_manager));
        // cereal::make_nvp("Default App", reinterpret_cast<DefaultApp>(*this)));
    }
};