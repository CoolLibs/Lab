#pragma once
#include "CommandCore/CommandExecutor_TopLevel.h"
#include "CommandCore/CommandExecutor_WithoutHistory_Ref.h"
#include "CommandCore/ReversibleCommandExecutor_WithoutHistory_Ref.h"
#include "Cool/AppManager/IApp.h"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/DebugOptions/DebugOptionsManager.h"
#include "Cool/Exporter/Polaroid.hpp"
#include "Cool/Mesh/MeshExportSettings.hpp"
#include "Cool/Midi/MidiManager.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/OSC/OSCManager.h"
#include "Cool/Server/ServerManager.hpp"
#include "Cool/Tips/TipsManager.h"
#include "Cool/View/ForwardingOrTextureView.hpp"
#include "Cool/View/TextureView.hpp"
#include "Cool/View/ViewsManager.h"
#include "Cool/Webcam/WebcamsConfigs.hpp"
#include "Cool/Window/WindowManager.h"
#include "Debug/DebugOptions.h"
#include "Gallery/GalleryPoster.h"
#include "Meshing/MeshingGui.hpp"
#include "Module/ShaderBased/DataToGenerateShaderCode.hpp"
#include "Module/ShaderBased/DataToPassToShader.hpp"
#include "Nodes/NodesLibraryManager.h"
#include "ProjectManager/Command_SaveProject.h"
#include "ProjectManager/Interfaces.hpp"
#include "ProjectManager/ProjectManager.hpp"
#include "ProjectManager/ProjectManagerImpl.hpp"
#include "no_sleep/no_sleep.hpp"

namespace Lab {

using DebugOptionsManager = Cool::DebugOptionsManager<
    Lab::DebugOptions,
    Cool::DebugOptions>;

class App : public Cool::IApp {
public:
    App(Cool::WindowManager& windows, Cool::ViewsManager& views);
    void init() override;
    void on_shutdown() override;
    void on_project_loaded();

    void update() override;
    void request_rerender() override;
    bool inputs_are_allowed() const override;
    bool wants_to_show_menu_bar() const override;

    void imgui_windows() override;
    void imgui_menus() override;

    void open_image_exporter();
    void open_video_exporter();
    void open_meshing_window_for_node(Cool::NodeId const& node_id);

    auto nodes_library() const -> Cool::NodesLibrary const& { return _nodes_library_manager.library(); }

private:
    void render(img::Size size, Cool::Time time, Cool::Time delta_time);
    void on_time_changed();
    void on_time_reset();
    auto render_view() -> Cool::TextureView&;
    void on_image_export_start(std::filesystem::path const& exported_image_path);
    void save_project_thumbnail();
    void save_project_thumbnail_impl(std::filesystem::path const& folder_path);

    void check_inputs();
    void check_inputs__history();
    void check_inputs__project();
    void check_inputs__timeline();

    // clang-format off
    auto make_reversible_commands_context           () const { return MakeReversibleCommandContext_Ref{{ project().camera_3D_manager, *project().modules_graph}}; }
    auto command_execution_context                  () -> CommandExecutionContext_Ref { return CommandExecutionContext_Ref{{*this, _project_manager, command_executor_top_level() }}; }
    auto reversible_command_executor_without_history() { return ReversibleCommandExecutor_WithoutHistory_Ref{command_execution_context()}; }
    auto command_executor_without_history           () const { return CommandExecutor_WithoutHistory_Ref{}; }
    auto command_executor_top_level                 () -> CommandExecutor_TopLevel { return CommandExecutor_TopLevel{command_executor_without_history(), project().history, make_reversible_commands_context()}; }
    auto command_executor                           () { return CommandExecutor{command_execution_context()}; }
    auto system_values                              (img::Size render_target_size, Cool::Time time, Cool::Time delta_time) const { return SystemValues{render_target_size, time, delta_time, project().camera_2D_manager.camera(), project().camera_3D_manager.camera(), project().audio}; }
    auto ui                                         () { return Ui_Ref{command_executor()}; }
    auto data_to_pass_to_shader                     (img::Size render_target_size, Cool::Time time, Cool::Time delta_time) const { return DataToPassToShader{system_values(render_target_size, time, delta_time),  project().modules_graph->graph() }; }
    auto data_to_generate_shader_code               () const { return DataToGenerateShaderCode{project().modules_graph->graph(), Cool::GetNodeDefinition_Ref<NodeDefinition>{_nodes_library_manager.library()} }; }
    // clang-format on

    Cool::Polaroid polaroid();

    void reset_cameras();

    void file_menu();
    void performance_menu();
    void export_menu();
    void settings_menu();
    void commands_menu();
    void debug_menu();

    void imgui_windows_only_when_inputs_are_allowed();
    void imgui_window_view();
    void imgui_window_exporter();
    void imgui_window_meshing();

public: // Needs to be public so we can call it in the NodesConfig
    void imgui_window_cameras();
    auto make_window_title_setter() -> SetWindowTitle;
    auto make_on_project_loaded() -> OnProjectLoaded;
    auto make_save_thumbnail() -> SaveThumbnail;

private:
    void compile_all_is0_nodes();
    auto project() -> Project& { return _project_manager.project(); }
    auto project() const -> Project const& { return _project_manager.project(); }

private:
    Cool::Window&                   _main_window;
    Cool::TextureView&              _output_view;
    Cool::ForwardingOrTextureView&  _preview_view; // Must be after _output_view because it stores a reference to it
    ProjectManager                  _project_manager{};
    Cool::Time                      _last_time{0s};
    bool                            _wants_view_in_fullscreen{false}; // Boolean that anyone can set to true or false at any moment to toggle the view's fullscreen mode.
    bool                            _view_was_in_fullscreen_last_frame{false};
    GalleryPoster                   _gallery_poster{};
    Cool::TipsManager               _tips_manager{};
    NodesLibraryManager             _nodes_library_manager{};
    bool                            _is_shutting_down{false};
    Cool::MeshExportSettings        _mesh_export_settings{};
    MeshingGui                      _meshing_gui{};
    std::optional<no_sleep::Scoped> _disable_sleep{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive, class AppT>
    static void serialize_impl(Archive& archive, AppT&& app) // Template to allow us to use it for both App& and App const&.
    {
        archive(
            ser20::make_nvp("Gallery Poster", app._gallery_poster),
            ser20::make_nvp("Tips", app._tips_manager),
            ser20::make_nvp("Output view", app._output_view),
            ser20::make_nvp("Webcams config", Cool::WebcamsConfigs::instance()),
            ser20::make_nvp("MIDI config", Cool::midi_manager()),
            ser20::make_nvp("OSC config", Cool::osc_manager()),
            ser20::make_nvp("Server config", Cool::server_manager()),
            ser20::make_nvp("3D Model export settings", app._mesh_export_settings),
            ser20::make_nvp("3D Model generation", app._meshing_gui)
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
