#pragma once

#include <Cool/AppManager/IApp.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Exporter/internal/Polaroid.h>
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Time/Clock_Realtime.h>
#include <Cool/View/RenderableViewManager.h>
#include <Cool/Window/WindowManager.h>
#include <reg/cereal.hpp>
#include "CameraManager.h"
#include "Dependencies/Dirty.h"
#include "Dependencies/History.h"
#include "Dependencies/Registries.h"
#include "Module.h"
#include "ShaderManager/ShaderManagerManager.h"
#include "UI/ThemeManager.h"

namespace Lab {

class App : public Cool::IApp {
public:
    explicit App(Cool::WindowManager& windows);
    ~App();

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
    void render(Cool::RenderTarget& render_target, float time);
    void render_impl(Cool::RenderTarget&, Module&, float time);

    // clang-format off
    auto all_input_slots() -> AllInputSlots;
    auto set_dirty_flag              () { return SetDirtyFlag{_dirty_registry}; }
    auto set_variable_dirty          () { return SetVariableDirty{all_input_slots(), set_dirty_flag()}; }
    auto commands_execution_context  () { return CommandExecutionContext{{_history, _registries, set_variable_dirty()}}; }
    auto reversible_commands_executor() { return ReversibleCommandExecutor{commands_execution_context()}; }
    auto commands_executor           () { return CommandExecutor{commands_execution_context()}; }
    auto commands_dispatcher         () { return CommandDispatcher{commands_executor(), _history, _registries}; }
    auto ui                          () { return Ui{_registries, commands_dispatcher(), set_dirty_flag()}; }
    auto input_provider              (float render_target_aspect_ratio, float time) { return InputProvider{_registries, render_target_aspect_ratio, time, _camera.id()}; }
    auto input_slot_destructor       () { return InputSlotDestructorRef{_registries}; }
    auto dirty_flag_factory          () { return DirtyFlagFactory{_dirty_registry}; }
    auto dirty_manager               () { return DirtyManager{_dirty_registry}; }
    // clang-format on

    Cool::Polaroid polaroid();

    void menu_preview();
    void menu_windows();
    void menu_export();
    void menu_settings();

    bool aspect_ratio_is_constrained() const;
    template<typename Event>
    Cool::ViewEvent<Event> view_event(const Event& event, const Cool::RenderableView& view)
    {
        return {event,
                _main_window.glfw(),
                {view.render_target.current_size(),
                 aspect_ratio_is_constrained()}};
    }

private:
    Registries                  _registries; // First because modules need the registries when they get created
    Cool::Window&               _main_window;
    CameraManager               _camera;
    Cool::Clock_Realtime        _clock;
    Cool::ImageSizeConstraint   _preview_constraint;
    Cool::RenderableViewManager _views; // Must be before the views because it is used to create them
    Cool::RenderableView&       _view;
    Cool::Exporter              _exporter;

    DirtyRegistry           _dirty_registry; // Before the modules because it is used to create them
    reg::Id<int>            _intId;
    ShaderManagerManager    _shader_manager;
    std::unique_ptr<Module> _current_module;
    std::unique_ptr<Module> _current_module2;
    History                 _history{};
    ThemeManager            _theme_manager{};
    Cool::RenderableView&   _view2;
    float                   _last_time{0.f};

#if DEBUG
    bool _show_imgui_debug = true;
    bool _show_imgui_demo  = false;
#endif

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Shader Manager Manager", _shader_manager),
                cereal::make_nvp("Registries", _registries),
                cereal::make_nvp("Dirty Registry", _dirty_registry),
                cereal::make_nvp("Int ID", _intId),
                cereal::make_nvp("History", _history),
                cereal::make_nvp("Module", _current_module),
                cereal::make_nvp("Module2", _current_module2),
                cereal::make_nvp("Camera Manager", _camera));
    }
#if !IS0_TEST_NODES
    // Must be declared last because its constructor modifies App, and its destructor requires all other members to still be alive
    Cool::AutoSerializer<App> _auto_serializer{Cool::Path::root() + "/last-session-cache.json", "App", *this};
#endif
};

} // namespace Lab