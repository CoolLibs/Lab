#pragma once

#include <Cool/AppManager/IApp.h>
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/DebugOptions/DebugOptionsManager.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Exporter/internal/Polaroid.h>
#include <Cool/Gpu/OpenGL/Texture.h>
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/MessageConsole/MessageConsole.h>
#include <Cool/Path/Path.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Time/Clock_Realtime.h>
#include <Cool/View/RenderableViewManager.h>
#include <Cool/Window/WindowManager.h>
#include <reg/cereal.hpp>
#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/CommandLogger.h"
#include "Commands/Command_SetCameraZoom.h" // For the serialization functions
#include "Debug/DebugOptions.h"
#include "Debug/TestMessageConsole.h"
#include "Dependencies/CameraManager.h"
#include "Dependencies/Dirty.h"
#include "Dependencies/History.h"
#include "Dependencies/Module.h"
#include "Dependencies/UpdateContext_Ref.h"
#include "Dependencies/VariableRegistries.h"
#include "Module_CustomShader/Module_CustomShader.h"
#include "Module_is0/Module_is0.h"
#include "UI/ThemeManager.h"

namespace Lab {

#if DEBUG
using DebugOptionsManager = Cool::DebugOptionsManager<
    Lab::DebugOptions,
    Cool::DebugOptions>;
#endif

class App : public Cool::IApp {
public:
    explicit App(Cool::WindowManager& windows);
    ~App();

    void update() override;
    bool inputs_are_allowed() const override;
    bool wants_to_show_menu_bar() const override;

    void imgui_windows() override;
    void imgui_menus() override;

    void on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event) override;

private:
    void render(Cool::RenderTarget& render_target, float time);
    void render_one_module(Module&, Cool::RenderTarget&, float time);
    void render_is0(Cool::RenderTarget& render_target, float time, img::Size size);
    void render_custom_shader(Cool::RenderTarget& render_target, float time);

    void check_inputs();
    void check_inputs__history();
    void check_inputs__export_windows();

    // clang-format off
    auto all_inputs() -> AllInputRefsToConst;
    auto set_dirty_flag                             () { return SetDirty_Ref{_dirty_registry}; }
    auto set_variable_dirty                         () { return SetVariableDirty_Ref{all_inputs(), set_dirty_flag()}; }
    auto make_reversible_commands_context           () { return MakeReversibleCommandContext_Ref{{_variable_registries, _camera_manager}}; }
    auto command_execution_context                  () { return CommandExecutionContext_Ref{{_history, _variable_registries, _camera_manager, set_variable_dirty()}}; }
    auto reversible_command_executor_without_history() { return ReversibleCommandExecutor_WithoutHistory_Ref{command_execution_context(), _command_logger}; }
    auto command_executor_without_history           () { return CommandExecutor_WithoutHistory_Ref{command_execution_context(), _command_logger}; }
    auto command_executor                           () { return CommandExecutor_TopLevel_Ref{command_executor_without_history(), _history, make_reversible_commands_context()}; }
    auto ui                                         () { return Ui_Ref{_variable_registries, command_executor(), set_dirty_flag()}; }
    auto input_provider                             (float render_target_aspect_ratio, float time) { return InputProvider_Ref{_variable_registries, render_target_aspect_ratio, time}; }
    auto input_destructor                           () { return InputDestructor_Ref{_variable_registries}; }
    auto input_factory                              () { return InputFactory_Ref{_variable_registries, _camera_manager.id()}; }
    auto dirty_flag_factory                         () { return DirtyFlagFactory_Ref{_dirty_registry}; }
    auto is_dirty__functor                          () { return IsDirty_Ref{_dirty_registry}; }
    auto set_clean__functor                         () { return SetClean_Ref{_dirty_registry}; }
    auto update_context                             () { return UpdateContext_Ref{{Cool::Log::ToUser::console(), set_clean__functor()}}; }
    // clang-format on

    Cool::Polaroid polaroid();

    void menu_preview();
    void menu_windows();
    void menu_export();
    void menu_settings();
    void menu_debug();

    void imgui_commands_and_registries_debug_windows();

    template<typename Event>
    Cool::ViewEvent<Event> view_event(const Event& event, const Cool::RenderableView& view)
    {
        return {
            event,
            _main_window.glfw(),
            {view.render_target.current_size()}};
    }

private:
    VariableRegistries                   _variable_registries; // First because modules need the registries when they get created
    CameraManager                        _camera_manager;      // First because modules need the camera id when they get created
    Cool::Window&                        _main_window;
    Cool::Clock_Realtime                 _clock;
    Cool::ImageSizeConstraint            _preview_constraint;
    Cool::RenderableViewManager          _views; // Must be before the views because it is used to create them
    Cool::RenderableView&                _is0_view;
    Cool::RenderableView&                _custom_shader_view;
    Cool::Exporter                       _exporter;
    DirtyRegistry                        _dirty_registry; // Before the modules because it is used to create them
    History                              _history{};
    ThemeManager                         _theme_manager{};
    float                                _last_time{0.f};
    std::unique_ptr<Module_is0>          _is0_module;
    std::unique_ptr<Module_CustomShader> _custom_shader_module;
    CommandLogger                        _command_logger{};
    Cool::OpenGL::Texture                _texture;
#if DEBUG
    TestMessageConsole _test_message_console{};
#endif

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Variable Registries", _variable_registries),
            cereal::make_nvp("Dirty Registry", _dirty_registry),
            cereal::make_nvp("History", _history),
            cereal::make_nvp("is0 Module", _is0_module),
            cereal::make_nvp("Custom Shader Module", _custom_shader_module),
            cereal::make_nvp("Preview Constraint", _preview_constraint),
            cereal::make_nvp("Camera Manager", _camera_manager)
        );
    }
#if !IS0_TEST_NODES && DEBUG
    DebugOptionsManager::AutoSerializer _auto_serializer_for_debug_options{};
#endif
};

} // namespace Lab
