#pragma once
#include <Cool/AppManager/IApp.h>
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/DebugOptions/DebugOptionsManager.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Exporter/internal/Polaroid.h>
#include <Cool/Gpu/OpenGL/Texture.h>
#include <Cool/Gpu/RenderTarget.h>
#include <Cool/Path/Path.h>
#include <Cool/Time/Clock_Realtime.h>
#include <Cool/View/RenderView.h>
#include <Cool/View/ViewsManager.h>
#include <Cool/Window/WindowManager.h>
#include <Nodes/NodesLibraryManager.h>
#include <ProjectManager/Command_SaveProject.h>
#include <ProjectManager/RecentlyOpened.h>
#include <reg/cereal.hpp>
#include "CommandCore/CommandExecutor_TopLevel.h"
#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/ReversibleCommandExecutor_WithoutHistory_Ref.h"
#include "Commands/Command_SetCameraZoom.h" // For the serialization functions
#include "Cool/Midi/MidiChannel.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/OSC/OSCManager.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/Tips/TipsManager.h"
#include "Cool/View/ForwardingOrRenderView.h"
#include "Cool/Webcam/WebcamsConfigs.h"
#include "Debug/DebugOptions.h"
#include "Dependencies/History.h"
#include "Dependencies/UpdateContext_Ref.h"
#include "Gallery/GalleryPoster.h"
#include "Project.h"

namespace Lab {

using DebugOptionsManager = Cool::DebugOptionsManager<
    Lab::DebugOptions,
    Cool::DebugOptions>;

class App : public Cool::IApp {
public:
    explicit App(Cool::WindowManager& windows, Cool::ViewsManager& views);
    ~App();
    void on_shutdown() override;

    void update() override;
    void request_rerender() override;
    bool inputs_are_allowed() const override;
    bool wants_to_show_menu_bar() const override;

    void imgui_windows() override;
    void imgui_menus() override;

    void open_image_exporter();
    void open_video_exporter();

private:
    void render(Cool::RenderTarget& render_target, float time, float delta_time);
    void on_time_changed();
    void on_time_reset();
    auto render_view() -> Cool::RenderView&;

    void check_inputs();
    void check_inputs__history();
    void check_inputs__project();
    void check_inputs__timeline();

    // clang-format off
    auto make_reversible_commands_context           () { return MakeReversibleCommandContext_Ref{{ _project.camera_manager}}; }
    auto command_execution_context                  () ->CommandExecutionContext_Ref { return CommandExecutionContext_Ref{{*this, _project.history, _project.camera_manager, _main_window, _project, _current_project_path, command_executor_top_level(), _recently_opened_projects }}; }
    auto reversible_command_executor_without_history() { return ReversibleCommandExecutor_WithoutHistory_Ref{command_execution_context()}; }
    auto command_executor_without_history           () { return CommandExecutor_WithoutHistory_Ref{}; }
    auto command_executor_top_level                 () -> CommandExecutor_TopLevel { return CommandExecutor_TopLevel{command_executor_without_history(), _project.history, make_reversible_commands_context()}; }
    auto command_executor                           () { return CommandExecutor{command_execution_context()}; }
    auto input_provider                             (float render_target_aspect_ratio, float height, float time, float delta_time) { return InputProvider_Ref{render_target_aspect_ratio, height, time, delta_time, _project.camera2D.value().transform_matrix(), _project.camera_manager.camera(), _project.audio}; }
    auto ui                                         () { return Ui_Ref{command_executor(), _project.audio}; }
    auto update_context                             () { return UpdateContext_Ref{{ui(), _nodes_library_manager.library()}}; }
    // clang-format on

    Cool::Polaroid polaroid();

    void reset_cameras();

    void file_menu();
    void view_menu();
    // void windows_menu();
    void export_menu();
    void settings_menu();
    void commands_menu();
    void debug_menu();

    void imgui_windows_only_when_inputs_are_allowed();
    void imgui_window_cameras();
    void imgui_window_view();
    void imgui_window_exporter();

    void imgui_commands_debug_windows();

    void compile_all_is0_nodes();

private:
    Cool::Window&                        _main_window;
    Cool::RenderView&                    _output_view;
    Cool::ForwardingOrRenderView&        _preview_view; // Must be after _output_view because it stores a reference to it
    Project                              _project{};
    std::optional<std::filesystem::path> _current_project_path{};
    RecentlyOpened                       _recently_opened_projects{};
    float                                _last_time{0.f};
    bool                                 _wants_view_in_fullscreen{false}; // Boolean that anyone can set to true or false at any moment to toggle the view's fullscreen mode.
    bool                                 _view_was_in_fullscreen_last_frame{false};
    GalleryPoster                        _gallery_poster{};
    Cool::TipsManager                    _tips_manager{};
    NodesLibraryManager                  _nodes_library_manager{};
    bool                                 _is_first_frame{true};
    bool                                 _is_shutting_down{false};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive, class AppT>
    static void serialize_impl(Archive& archive, AppT&& app) // Template to allow us to use it for both App& and App const&.
    {
        archive(
            cereal::make_nvp("Recently opened projects", app._recently_opened_projects),
            cereal::make_nvp("Gallery Poster", app._gallery_poster),
            cereal::make_nvp("Tips", app._tips_manager),
            cereal::make_nvp("Output view", app._output_view),
            cereal::make_nvp("Webcams config", Cool::WebcamsConfigs::instance()),
            cereal::make_nvp("MIDI config", Cool::midi_manager()),
            cereal::make_nvp("OSC config", Cool::osc_manager())
        );
    }
    template<class Archive>
    void save(Archive& archive) const
    {
        serialize_impl(archive, *this);
        if (!_is_shutting_down)
            const_cast<App&>(*this).command_executor().execute(Command_SaveProject{.is_autosave = true}); // NOLINT(cppcoreguidelines-pro-type-const-cast) This is not UB because no one will ever create a const App.
        // else: The project has already been saved during App::on_shutdown(), no need to save it here.
    }
    template<class Archive>
    void load(Archive& archive)
    {
        serialize_impl(archive, *this);
    }
    DebugOptionsManager::AutoSerializer _auto_serializer_for_debug_options{};
};

} // namespace Lab
