#pragma once

#include <Cool/AppManager/IApp.h>
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/DebugOptions/DebugOptionsManager.h>
#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/VariableRegistries.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Exporter/internal/Polaroid.h>
#include <Cool/Gpu/OpenGL/Texture.h>
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/Nodes/ImNodes_StyleEditor.h>
#include <Cool/Path/Path.h>
#include <Cool/Time/Clock_Realtime.h>
#include <Cool/View/RenderableViewManager.h>
#include <Cool/Window/WindowManager.h>
#include <reg/cereal.hpp>
#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/CommandLogger.h"
#include "Commands/Command_SetCameraZoom.h" // For the serialization functions
#include "Cool/ImGui/StyleEditor.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Debug/DebugOptions.h"
#include "Dependencies/CameraManager.h"
#include "Dependencies/History.h"
#include "Dependencies/Module.h"
#include "Dependencies/UpdateContext_Ref.h"
#include "Module_Nodes/Module_Nodes.h"

namespace Lab {

using DebugOptionsManager = Cool::DebugOptionsManager<
    Lab::DebugOptions,
    Cool::DebugOptions>;

class App : public Cool::IApp {
public:
    explicit App(Cool::WindowManager& windows);
    ~App();

    void update() override;
    void trigger_rerender() override;
    bool inputs_are_allowed() const override;
    bool wants_to_show_menu_bar() const override;

    void imgui_windows() override;
    void imgui_menus() override;

    void on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event) override;

    void open_image_exporter();
    void open_video_exporter();

private:
    void render(Cool::RenderTarget& render_target, float time);
    void render_one_module(Module&, Cool::RenderTarget&, float time);
    void render_nodes(Cool::RenderTarget& render_target, float time, img::Size size);

    void check_inputs();
    void check_inputs__history();
    void check_inputs__export_windows();
    void check_inputs__timeline();

    // clang-format off
    auto all_inputs() -> Cool::AllInputRefsToConst;
    auto set_dirty_flag                             () { return Cool::SetDirty_Ref{_dirty_registry}; }
    auto set_variable_dirty                         () { return Cool::SetVariableDirty_Ref{all_inputs(), set_dirty_flag()}; }
    auto make_reversible_commands_context           () { return MakeReversibleCommandContext_Ref{{_variable_registries, _camera_manager}}; }
    auto command_execution_context                  () { return CommandExecutionContext_Ref{{*this, _history, _variable_registries, _camera_manager, set_variable_dirty() }}; }
    auto reversible_command_executor_without_history() { return ReversibleCommandExecutor_WithoutHistory_Ref{command_execution_context(), _command_logger}; }
    auto command_executor_without_history           () { return CommandExecutor_WithoutHistory_Ref{command_execution_context(), _command_logger}; }
    auto command_executor                           () { return CommandExecutor_TopLevel_Ref{command_executor_without_history(), _history, make_reversible_commands_context()}; }
    auto input_provider                             (float render_target_aspect_ratio,float height, float time, glm::mat3 const& cam2D) { return Cool::InputProvider_Ref{_variable_registries, render_target_aspect_ratio, height, time, cam2D}; }
    auto input_factory                              () { return Cool::InputFactory_Ref{_variable_registries, _camera_manager.id()}; }
    auto ui                                         () { return Ui_Ref{_variable_registries, command_executor(), set_dirty_flag(), input_factory()}; }
    auto dirty_flag_factory                         () { return Cool::DirtyFlagFactory_Ref{_dirty_registry}; }
    auto is_dirty__functor                          () { return Cool::IsDirty_Ref{_dirty_registry}; }
    auto set_clean__functor                         () { return Cool::SetClean_Ref{_dirty_registry}; }
    auto set_dirty__functor                         () { return Cool::SetDirty_Ref{_dirty_registry}; }
    auto update_context                             () { return UpdateContext_Ref{{Cool::Log::ToUser::console(), set_clean__functor(), set_dirty__functor(), input_provider(0.f, 0.f, -100000.f, glm::mat3{1.f} /* HACK: Dummy values, they should not be needed. Currently this is only used by shader code generation to inject of very specific types like Gradient */), ui()}}; }
    // clang-format on

    Cool::Polaroid polaroid();

    void reset_cameras();

    void view_menu();
    // void windows_menu();
    void export_menu();
    void settings_menu();
    void debug_menu();

    void imgui_windows_only_when_inputs_are_allowed();
    void cameras_window();

    void imgui_commands_and_registries_debug_windows();

    template<typename Event>
    Cool::ViewEvent<Event> view_event(const Event& event, const Cool::RenderableView& view)
    {
        return {
            event,
            _main_window.glfw(),
            {view.render_target.current_size()}};
    }

    void compile_all_is0_nodes();
    void set_everybody_dirty();

private:
    Cool::VariableRegistries       _variable_registries; // First because modules need the registries when they get created
    CameraManager                  _camera_manager;      // First because modules need the camera id when they get created
    Cool::Variable<Cool::Camera2D> _camera2D;
    Cool::Window&                  _main_window;
    Cool::Clock_Realtime           _clock;
    Cool::ImageSizeConstraint      _view_constraint;
    Cool::RenderableViewManager    _views; // Must be before the views because it is used to create them
    Cool::RenderableView&          _nodes_view;
    Cool::Exporter                 _exporter;
    Cool::DirtyRegistry            _dirty_registry; // Before the modules because it is used to create them
    History                        _history{};
    float                          _last_time{0.f};
    std::unique_ptr<Module_Nodes>  _nodes_module;
    CommandLogger                  _command_logger{};
    bool                           _is_first_frame{true};
    Cool::ImNodes_StyleEditor      _imnodes_style{};
    Cool::StyleEditor              _style{};
    bool                           _is_camera_2D_locked_in_view{false};
    bool                           _wants_view_in_fullscreen{false}; // Boolean that anyone can set to true or false at any moment to toggle the view's fullscreen mode.
    bool                           _view_was_in_fullscreen_last_frame{false};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Is camera 2D locked in view", _is_camera_2D_locked_in_view),
            cereal::make_nvp("Camera Manager", _camera_manager),
            cereal::make_nvp("Dirty Registry", _dirty_registry),
            cereal::make_nvp("Variable Registries", _variable_registries),
            cereal::make_nvp("View Constraint", _view_constraint),
            cereal::make_nvp("Exporter (Image and Video)", _exporter),
            cereal::make_nvp("Camera 2D", _camera2D),
            cereal::make_nvp("History", _history),
            cereal::make_nvp("Nodes Module", _nodes_module),
            cereal::make_nvp("ImNodes style", _imnodes_style)
        );
    }
    DebugOptionsManager::AutoSerializer _auto_serializer_for_debug_options{};
};

} // namespace Lab
