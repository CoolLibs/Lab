#pragma once

#include <Cool/AppManager/IApp.h>
#include <Cool/Camera/CameraManager.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Exporter/internal/Polaroid.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Image/ImageSizeConstraint.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Time/Clock_Realtime.h>
#include <Cool/View/RenderableViewManager.h>
#include <Cool/Window/WindowManager.h>
#include "ShaderManager/ShaderManagerManager.h"

using namespace Cool;

class App : public IApp {
public:
    explicit App(Cool::WindowManager& windows);

    void update() override;
    bool inputs_are_allowed() const override;
    bool wants_to_show_menu_bar() const override;
    void imgui_windows() override;
    void imgui_menus() override;

    void on_keyboard_event(const Cool::KeyboardEvent& event) override;
    void on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event) override;

private:
    void           render(Cool::RenderTarget& render_target, float time);
    Cool::Polaroid polaroid();
    bool           aspect_ratio_is_constrained() const;
    template<typename Event>
    ViewEvent<Event> view_event(const Event& event, const RenderableView& view)
    {
        return {event,
                _main_window.glfw(),
                {view.render_target.current_size(),
                 aspect_ratio_is_constrained()}};
    }

private:
    Window&                     _main_window;
    ShaderManagerManager        _shader_manager;
    Cool::CameraManager         _camera;
    Cool::Clock_Realtime        _clock;
    Cool::ImageSizeConstraint   _preview_constraint;
    Cool::RenderableViewManager _views; // Must be before the views because it is used to construct them
    Cool::RenderableView&       _view;
    Exporter                    _exporter;
#ifdef DEBUG
    bool _show_imgui_debug = true;
    bool _show_imgui_demo  = false;
#endif
    // Must be declared last because its constructor modifies App, and its destructor requires all other members to still be alive
    Cool::AutoSerializer<App> _auto_serializer{Cool::File::root_dir() + "/last-session-cache.json", "App", *this};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Shader Manager Manager", _shader_manager),
                cereal::make_nvp("Camera Manager", _camera));
    }
};