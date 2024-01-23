#include "App.h"
#include <Cool/DebugOptions/TestMessageConsole.h>
#include <Cool/DebugOptions/TestPresets.h>
#include <Cool/Gpu/TextureLibrary_FromFile.h>
#include <Cool/ImGui/Fonts.h>
#include <Cool/ImGui/icon_fmt.h>
#include <Cool/ImGui/test_markdown_formatting.h>
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>
#include <Cool/Time/ClockU.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Variables/TestVariables.h>
#include <Cool/Webcam/TextureLibrary_FromWebcam.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h>
#include <ModulesGraph/ModulesGraph.h>
#include <ProjectManager/Command_OpenProject.h>
#include <ProjectManager/Command_PackageProjectInto.h>
#include <ProjectManager/utils.h>
#include <Tips/Tips.h>
#include <chrono>
#include <cmd/imgui.hpp>
#include <filesystem>
#include <reg/src/internal/generate_uuid.hpp>
#include <stringify/stringify.hpp>
#include "CommandCore/command_to_string.h"
#include "Commands/Command_OpenImageExporter.h"
#include "Commands/Command_OpenVideoExporter.h"
#include "Common/Path.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Log/Message.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/Tips/TipsManager.h"
#include "Cool/Tips/test_tips.h"
#include "Cool/View/View.h"
#include "Cool/View/ViewsManager.h"
#include "Cool/Webcam/WebcamsConfigs.h"
#include "Debug/DebugOptions.h"
#include "Dependencies/Camera2DManager.h"
#include "Dump/gen_dump_string.h"
#include "Menus/about_menu.h"
#include "ProjectManager/Command_NewProject.h"
#include "ProjectManager/Command_OpenBackupProject.h"
#include "Tips/Tips.h"
#include "UI/imgui_show.h"
#include "img/img.hpp"
#include "imgui.h"

namespace Lab {

App::App(Cool::WindowManager& windows, Cool::ViewsManager& views)
    : _main_window{windows.main_window()}
    , _output_view{views.make_view<Cool::RenderView>(Cool::ViewCreationParams{
          .name        = Cool::icon_fmt("Output", ICOMOON_IMAGE),
          .is_closable = true,
          .start_open  = false,
      })}
    , _preview_view{views.make_view<Cool::ForwardingOrRenderView>(
          _output_view,
          Cool::ViewCreationParams{
              .name        = Cool::icon_fmt("View", ICOMOON_IMAGE),
              .is_closable = false,
              .start_open  = true,
          }
      )}
{
    command_executor().execute(Command_NewProject{});
    _project.clock.pause(); // Make sure the new project will be paused.

    _project.camera_manager.hook_events(_preview_view.mouse_events(), _project.variable_registries, command_executor(), [this]() { trigger_rerender(); });
    Cool::midi_manager().set_additional_midi_callback([&]() {
        trigger_rerender();
    });
    hook_camera2D_events(
        _preview_view.mouse_events(),
        _project.camera2D.value(),
        [this]() { trigger_rerender(); },
        [this]() { return !_project.is_camera_2D_editable_in_view; }
    );

    // serv::init([](std::string_view request) {
    //     Cool::Log::Debug::info("Scripting", "{}", request);
    // });
}

App::~App()
{
    // serv::shut_down();
}

void App::on_shutdown()
{
    _tips_manager.on_app_shutdown();
    command_execution_context().execute(Command_SaveProject{.is_autosave = true});
    _is_shutting_down = true;
}

void App::compile_all_is0_nodes()
{
    // for (const auto& node_template : _project.modules_graph->compositing_module().nodes_templates())
    // {
    //     _project.modules_graph->compositing_module().remove_all_nodes();
    //     Cool::Log::ToUser::info("Test is0 Node", node_template.name);
    //     _project.modules_graph->compositing_module().add_node(NodeFactoryU::node_from_template(node_template));
    //     _project.modules_graph->compositing_module().recompile(update_context(), true);
    // }
    // _project.modules_graph->compositing_module().remove_all_nodes();
}

void App::set_everybody_dirty()
{
    std::unique_lock lock{_project.dirty_registry.mutex()};
    for (auto& [_, is_dirty] : _project.dirty_registry)
        is_dirty.is_dirty = true;
}

void App::on_time_changed()
{
    _project.modules_graph->on_time_changed(update_context());
}

void App::on_time_reset()
{
    _project.modules_graph->on_time_reset();
}

void App::update()
{
    // First frame the exe is open
    // Since the construction of an App might be in two steps (constructor, and then deserialization)
    // we do our actual construction logic here, to make sure it is done once and only once.
    if (_is_first_frame)
    {
        _is_first_frame = false;
        initial_project_opening(command_execution_context());
    }
    // First frame a project is loaded
    if (_project.is_first_frame)
    {
        _project.is_first_frame = false;
        set_everybody_dirty();
    }

    Cool::user_settings().color_themes.update();

    _project.audio.set_force_mute(_project.exporter.is_exporting());
    _project.audio.sync_with_clock(
        _project.exporter.is_exporting()
            ? _project.exporter.clock()
            : _project.clock,
        _project.exporter.is_exporting() /* force_sync_time */
    );
    _project.audio.update(/*on_audio_data_changed = */ [&]() {
        _project.modules_graph->on_audio_changed(update_context());
    });

    if (inputs_are_allowed()) // Must update() before we render() to make sure the modules are ready (e.g. Nodes need to parse the definitions of the nodes from files)
    {
        _nodes_library_manager.update(update_context(), _project.modules_graph->regenerate_code_flag(), _project.modules_graph->graph(), _project.modules_graph->nodes_config(ui(), _nodes_library_manager.library()));
        _project.modules_graph->update(update_context());
        // _custom_shader_module->update(update_context());
        check_inputs();
    }

    if (!_project.exporter.is_exporting())
    {
        _project.clock.update();
        render_view().update_size(_project.view_constraint); // TODO(JF) Integrate the notion of View Constraint inside the RenderView ? But that's maybe too much coupling
        polaroid().render(_project.clock.time(), _project.clock.delta_time());
    }
    else
    {
        trigger_rerender();
        _project.exporter.update(polaroid());
    }

    // if (_custom_shader_view.render_target.needs_resizing())
    // {
    // set_dirty_flag()(_custom_shader_module->dirty_flag());
    // }

    if (DebugOptions::copy_info_dump_to_clipboard())
    {
        auto const string = gen_dump_string();
        ImGui::SetClipboardText(string.c_str());
        Cool::Log::ToUser::info("Info Dump", fmt::format("Info dump has been successfully copied to clipboard:\n\n{}", string));
    }
    if (DebugOptions::generate_dump_file())
    {
        auto const path   = Cool::Path::user_data() / "info_dump.txt";
        auto const string = gen_dump_string();
        Cool::File::set_content(path, string);
        Cool::Log::ToUser::info(
            "Info Dump",
            fmt::format("Info dump has been successfully generated in {}:\n\n{}", path, string),
            std::vector{
                Cool::ClipboardContent{
                    .title   = "folder path",
                    .content = path.parent_path().string(),
                },
                Cool::ClipboardContent{
                    .title   = "file path",
                    .content = path.string(),
                },
            }
        );
    }
}

void App::trigger_rerender() // TODO(Modules) Sometimes we don't need to call this, but only rerender a specific module instead
{
    _project.modules_graph->trigger_rerender_all(set_dirty_flag());
}

auto App::all_inputs() -> Cool::AllInputRefsToConst
{
    // auto vec  = _custom_shader_module->all_inputs();
    auto vec2 = _project.modules_graph->all_inputs();
    // for (const auto& x : vec2)
    // {
    //     vec.push_back(x);
    // }
    // return vec;
    return vec2;
}

auto App::render_view() -> Cool::RenderView&
{
    if (_output_view.is_open())
        return _output_view;
    return _preview_view;
}

Cool::Polaroid App::polaroid()
{
    return {
        .render_target = render_view().render_target(), // TODO(Modules) Each module should have its own render target that it renders on. The views shouldn't have a render target, but receive the one of the top-most module by reference.
        .render_fn     = [this](Cool::RenderTarget& render_target, float time, float delta_time) {
            if (_last_time != time)
            {
                _last_time = time;
                on_time_changed();
            }
            render(render_target, time, delta_time);
        }
    };
}

auto App::inputs_are_allowed() const -> bool
{
    return !_project.exporter.is_exporting();
}

auto App::wants_to_show_menu_bar() const -> bool
{
    return !_project.exporter.is_exporting() && !_wants_view_in_fullscreen;
}

static void imgui_window_console()
{
    Cool::Log::ToUser::console().imgui_window();
#if DEBUG
    Cool::Log::Debug::console().imgui_window();
#endif
}

// void App::render_custom_shader(Cool::RenderTarget& render_target, float time)
// {
// _custom_shader_module->set_image_in_shader("_image", 0, _nodes_view.render_target().get().texture_id());
// render_one_module(*_custom_shader_module, render_target, time);
// }

void App::render(Cool::RenderTarget& render_target, float time, float delta_time)
{
    auto const aspect_ratio = img::SizeU::aspect_ratio(render_target.desired_size());
    _project.modules_graph->render(
        render_target,
        Module::RenderParams{
            input_provider(aspect_ratio, static_cast<float>(render_target.desired_size().height()), time, delta_time, _project.camera2D.value().transform_matrix()),
            input_factory(),
            is_dirty__functor(),
            set_clean__functor(),
            _project.variable_registries,
            render_target.desired_size(),
        },
        update_context(),
        dirty_flag_factory()
    );
}

void App::imgui_commands_and_registries_debug_windows()
{
    const auto the_ui = ui();
    the_ui.window({.name = "Registry of vec3"}, [&]() {
        imgui_show(_project.variable_registries.of<Cool::Variable<glm::vec3>>());
    });
    the_ui.window({.name = "Registry of float"}, [&]() {
        imgui_show(_project.variable_registries.of<Cool::Variable<float>>());
    });
    the_ui.window({.name = "Registry of int"}, [&]() {
        imgui_show(_project.variable_registries.of<Cool::Variable<int>>());
    });
    the_ui.window({.name = "Registry of Camera"}, [&]() {
        imgui_show(_project.variable_registries.of<Cool::Variable<Cool::Camera>>());
    });
    the_ui.window({.name = "Registry of DirtyFlag"}, [&]() {
        imgui_show(_project.dirty_registry);
    });
    the_ui.window({.name = "History"}, [&]() {
        _project.history.imgui_show([](const ReversibleCommand& command) {
            return command_to_string(command);
        });
    });
}

void App::imgui_window_cameras()
{
    static constexpr auto help_text = "When disabled, prevents you from changing your camera by clicking in the View. This can be useful when working with both 2D and 3D nodes: you don't want both the 2D and 3D cameras active at the same time.";

    ImGui::PushID("##2D");
    Cool::ImGuiExtras::separator_text("2D Camera");
    Cool::ImGuiExtras::toggle("Editable in view", &_project.is_camera_2D_editable_in_view);
    Cool::ImGuiExtras::help_marker(help_text);
    if (imgui_widget(_project.camera2D))
        trigger_rerender();
    ImGui::PopID();

    ImGui::NewLine();

    ImGui::PushID("##3D");
    Cool::ImGuiExtras::separator_text("3D Camera");
    Cool::ImGuiExtras::toggle("Editable in view", &_project.camera_manager.is_editable_in_view());
    Cool::ImGuiExtras::help_marker(help_text);
    _project.camera_manager.imgui(_project.variable_registries, command_executor(), [this]() { trigger_rerender(); });
    ImGui::PopID();
}

void App::imgui_window_view()
{
    bool const view_in_fullscreen = _project.exporter.is_exporting() || _wants_view_in_fullscreen;
    {
        if (!_view_was_in_fullscreen_last_frame && view_in_fullscreen)
            save_imgui_windows_state(); // Save normal state before making the View fullscreen.
        if (_view_was_in_fullscreen_last_frame && !view_in_fullscreen)
            restore_imgui_windows_state(); // After fullscreen, restore the normal state.
        _view_was_in_fullscreen_last_frame = view_in_fullscreen;
    }

    _project.modules_graph->submit_gizmos(_preview_view.gizmos_manager(), update_context());
    _output_view.imgui_window({
        .on_open  = [&]() { trigger_rerender(); }, // When we switch between using the _output_view and the _nodes_view
        .on_close = [&]() { trigger_rerender(); }, // as our render target, we need to rerender.
    });
    _preview_view.imgui_window({
        .fullscreen    = view_in_fullscreen,
        .extra_widgets = [&]() {
            if (_project.exporter.is_exporting())
                return false;
            bool b = false;

            bool const align_buttons_vertically = _preview_view.has_vertical_margins()
                                                  || (!_project.view_constraint.wants_to_constrain_aspect_ratio() && !_output_view.is_open()); // Hack to avoid flickering the alignment of the buttons when we are resizing the View

            int buttons_order{0};
            // Reset cameras
            if (Cool::ImGuiExtras::floating_button(ICOMOON_TARGET, buttons_order++, align_buttons_vertically))
            {
                reset_cameras();
            }
            b |= ImGui::IsItemActive();
            ImGui::SetItemTooltip("%s", "Reset 2D and 3D cameras");

            // Toggle fullscreen
            if (Cool::ImGuiExtras::floating_button(_wants_view_in_fullscreen ? ICOMOON_SHRINK : ICOMOON_ENLARGE, buttons_order++, align_buttons_vertically))
            {
                _wants_view_in_fullscreen = !_wants_view_in_fullscreen;
                _main_window.set_fullscreen(_wants_view_in_fullscreen);
            }
            b |= ImGui::IsItemActive();
            ImGui::SetItemTooltip("%s", _wants_view_in_fullscreen ? "Shrink the view" : "Expand the view");

            // Toggle 2D / 3D cameras
            if (Cool::ImGuiExtras::floating_button(_project.is_camera_2D_editable_in_view ? ICOMOON_CAMERA : ICOMOON_VIDEO_CAMERA, buttons_order++, align_buttons_vertically))
            {
                _project.is_camera_2D_editable_in_view        = !_project.is_camera_2D_editable_in_view;
                _project.camera_manager.is_editable_in_view() = !_project.is_camera_2D_editable_in_view; // Only allow one camera active at the same time.
            }
            b |= ImGui::IsItemActive();
            ImGui::SetItemTooltip("%s", _project.is_camera_2D_editable_in_view ? "2D camera is active" : "3D camera is active");
            return b;
        },
    });
}

void App::imgui_window_exporter()
{
    _project.exporter.imgui_windows({
        .polaroid          = polaroid(),
        .time              = _project.clock.time(),
        .delta_time        = _project.clock.delta_time(),
        .on_image_exported = [&](std::filesystem::path const& exported_image_path) {
            auto folder_path = exported_image_path;
            folder_path.replace_extension(); // Give project folder the same name as the image.
            command_executor().execute(Command_PackageProjectInto{
                .folder_path = folder_path,
            });
            //
        },
        .on_video_export_start                      = [&]() { on_time_reset(); },
        .widgets_in_window_video_export_in_progress = [&]() {
            ImGui::NewLine();
            ImGui::SeparatorText(Cool::icon_fmt("Did you know?", ICOMOON_BUBBLE).c_str());
            _tips_manager.imgui_show_one_tip(all_tips());
            //
        },
    });
}

void App::imgui_windows()
{
    imgui_window_view();
    imgui_window_exporter();
    imgui_window_console();
    if (inputs_are_allowed())
        imgui_windows_only_when_inputs_are_allowed();
}

void App::imgui_windows_only_when_inputs_are_allowed()
{
    const auto the_ui = ui();
    // Time
    ImGui::Begin(Cool::icon_fmt("Time", ICOMOON_STOPWATCH).c_str());
    Cool::ClockU::imgui_timeline(_project.clock, /* on_time_reset = */ [&]() { on_time_reset(); });
    ImGui::End();
    // Cameras
    ImGui::Begin(Cool::icon_fmt("Cameras", ICOMOON_CAMERA).c_str());
    imgui_window_cameras();
    ImGui::End();
    // Audio
    _project.audio.imgui_window();
    // Webcams
    Cool::WebcamsConfigs::instance().imgui_window();
    // Midi
    Cool::midi_manager().imgui_window_config();
    // Tips
    _tips_manager.imgui_windows(all_tips());
    // Nodes
    _project.modules_graph->imgui_windows(the_ui, update_context()); // Must be after cameras so that Inspector window is always preferred over Cameras in tabs.
    // Share online
    _gallery_poster.imgui_window([&](img::Size size) {
        auto the_polaroid = polaroid();
        the_polaroid.render(_project.clock.time(), _project.clock.delta_time(), size);
        auto const image = the_polaroid.render_target.download_pixels();
        return img::save_png_to_string(image);
    });
    // Recently opened projects
    _recently_opened_projects.imgui_window(command_execution_context());

    DebugOptions::show_framerate_window([&] {
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        _main_window.imgui_cap_framerate();
    });
    if (DebugOptions::show_imgui_demo_window())                         // Show the big demo window (Most of the sample code is
        ImGui::ShowDemoWindow(&DebugOptions::show_imgui_demo_window()); // in ImGui::ShowDemoWindow()! You can browse its code
                                                                        // to learn more about Dear ImGui!).
    if (DebugOptions::show_commands_and_registries_debug_windows())
    {
        imgui_commands_and_registries_debug_windows();
    }
    if (DebugOptions::show_nodes_and_links_registries())
    {
        _project.modules_graph->debug_show_nodes_and_links_registries_windows(ui());
    }

    Cool::DebugOptions::texture_library_debug_view([&] {
        Cool::TextureLibrary_FromFile::instance().imgui_debug_view();
    });
    DebugOptions::test_all_variable_widgets__window(&Cool::test_variables);
    DebugOptions::test_shaders_compilation__window([&]() {
        if (ImGui::Button("Compile everything"))
        {
            Cool::Log::ToUser::console().clear();
            compile_all_is0_nodes();
        }
        ImGui::Separator();
        if (ImGui::Button("Compile all is0 Nodes"))
        {
            Cool::Log::ToUser::console().clear();
            compile_all_is0_nodes();
        }
    });

    Cool::DebugOptions::test_message_console__window([]() {
        static auto test_message_console = Cool::TestMessageConsole{};
        test_message_console.imgui(
            Cool::Log::ToUser::console()
        );
    });

    Cool::DebugOptions::test_presets__window([]() {
        static auto test_presets = TestPresets{};
        test_presets.imgui();
    });

    Cool::DebugOptions::test_markdown_formatting_window([]() {
        Cool::test_markdown_formatting();
    });

    Cool::DebugOptions::emulate_midi_keyboard([]() {
        Cool::midi_manager().imgui_emulate_midi_keyboard();
    });

    Cool::DebugOptions::test_tips([this]() {
        test_tips(_tips_manager);
    });

    Cool::DebugOptions::color_themes_advanced_config_window([&]() {
        Cool::user_settings().color_themes.imgui_advanced_config();
    });

    Cool::DebugOptions::color_themes_editor([&]() {
        Cool::user_settings().color_themes.imgui_basic_theme_editor();
    });
    DebugOptions::empty_window([] {});
}

void App::file_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("File", ICOMOON_FILE_TEXT2, true).c_str()))
    {
        imgui_open_save_project(command_execution_context());
        ImGui::EndMenu();
    }
}

void App::view_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("View", ICOMOON_IMAGE, true).c_str()))
    {
        if (_project.view_constraint.imgui())
        {
            // render_impl(_view.render_target, *_current_module, _project.clock.time());
        }
        ImGui::EndMenu();
    }
}

// void App::windows_menu()
// {
//     if (ImGui::BeginMenu("Windows"))
//     {
//         for (auto& view : _views)
//         {
//             view.view.imgui_open_close_checkbox();
//         }
//         ImGui::EndMenu();
//     }
// }

void App::export_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("Export", ICOMOON_UPLOAD2, true).c_str()))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2{0.f, 0.5f});
        _project.exporter.imgui_menu_items(
            {
                .open_image_exporter = [&]() { command_executor().execute(Command_OpenImageExporter{}); },
                .open_video_exporter = [&]() { command_executor().execute(Command_OpenVideoExporter{}); },
            },
            Cool::icon_fmt("Share online", ICOMOON_EARTH, true)
        );
        _gallery_poster.imgui_open_sharing_form(_project.view_constraint.aspect_ratio());
        ImGui::PopStyleVar();
        ImGui::EndMenu();
    }
}

void App::settings_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("Settings", ICOMOON_COG, true).c_str()))
    {
        Cool::ImGuiExtras::separator_text("History");
        _project.history.imgui_max_size(&Cool::ImGuiExtras::help_marker);
        _project.history.imgui_max_saved_size(&Cool::ImGuiExtras::help_marker);

        Cool::ImGuiExtras::separator_text("Color Theme");
        Cool::user_settings().color_themes.imgui_theme_picker();

        Cool::user_settings().imgui();

        ImGui::EndMenu();
    }
}

void App::commands_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("Commands", ICOMOON_ROCKET, true).c_str()))
    {
        if (ImGui::Selectable(ICOMOON_BUBBLE " Show all the tips"))
            _tips_manager.open_all_tips_window();
        if (ImGui::Selectable(ICOMOON_VIDEO_CAMERA " Open webcams config"))
            Cool::WebcamsConfigs::instance().open_imgui_window();
        if (ImGui::Selectable(ICOMOON_EQUALIZER2 " Open MIDI config"))
            Cool::midi_manager().open_config_window();
        if (ImGui::Selectable(ICOMOON_MUSIC " Open Audio config"))
            _project.audio.open_imgui_window();
        if (ImGui::Selectable(ICOMOON_IMAGE " Open output window"))
            _output_view.open();
        ImGui::EndMenu();
    }
}

void App::debug_menu()
{
    static bool was_closed_last_frame{true}; // HACK(JF) I guess a `static` here is okay because no one is gonna want two distinct instances of the same debug menu O:) A better solution would be to make a small Menu class that would remember if it was open last frame or not.
    if (ImGui::BeginMenu("Debug"))
    {
        DebugOptionsManager::imgui_ui_for_all_options(was_closed_last_frame);
        was_closed_last_frame = false;
        ImGui::EndMenu();
    }
    else
    {
        was_closed_last_frame = true;
    }
}

void App::imgui_menus()
{
    file_menu();
    export_menu();
    // windows_menu();/// This menu might make sense if we have several views one day, but for now it just creates a menu for no reason
    view_menu();
    settings_menu();
    commands_menu();

    ImGui::SetCursorPosX( // HACK while waiting for ImGui to support right-to-left layout. See issue https://github.com/ocornut/imgui/issues/5875
        ImGui::GetWindowSize().x
        - ImGui::CalcTextSize("AboutDebug").x
        - 3.f * ImGui::GetStyle().ItemSpacing.x
        - ImGui::GetStyle().WindowPadding.x
    );
    about_menu();
    debug_menu();
}

void App::reset_cameras()
{
    _project.camera2D.value() = {}; // TODO(JF) Store this command in history
    _project.camera_manager.reset_camera(_project.variable_registries, command_executor(), [this]() { trigger_rerender(); });
}

void App::check_inputs()
{
    if (ImGui::GetIO().WantTextInput)
        return;

    check_inputs__history();
    check_inputs__project();
    check_inputs__timeline();
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        _wants_view_in_fullscreen = false;
    }
}

void App::check_inputs__history()
{
    auto        exec = reversible_command_executor_without_history();
    const auto& io   = ImGui::GetIO();

    // Undo
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z))
    {
        _project.history.move_backward(exec);
    }

    // Redo
    if ((io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y))
        || (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_Z)))
    {
        _project.history.move_forward(exec);
    }
}

void App::check_inputs__project()
{
    auto const& io = ImGui::GetIO();

    if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyReleased(ImGuiKey_S))
        dialog_to_save_project_as(command_execution_context());
    else if (io.KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_S))
        command_executor().execute(Command_SaveProject{});
    else if (io.KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_O))
        dialog_to_open_project(command_execution_context());
    else if (io.KeyCtrl && io.KeyShift && ImGui::IsKeyReleased(ImGuiKey_R))
        command_executor().execute(Command_OpenBackupProject{});
    else if (io.KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_R))
        dialog_to_open_recent_project(_recently_opened_projects);
    else if (io.KeyCtrl && ImGui::IsKeyReleased(ImGuiKey_N))
        command_executor().execute(Command_NewProject{});
}

void App::check_inputs__timeline()
{
    if (ImGui::IsKeyReleased(ImGuiKey_Space))
    {
        _project.clock.toggle_play_pause();
    }
}

void App::open_image_exporter()
{
    _project.exporter.maybe_set_aspect_ratio(_project.view_constraint.aspect_ratio());
    _project.exporter.image_export_window().open();
}

void App::open_video_exporter()
{
    _project.exporter.maybe_set_aspect_ratio(_project.view_constraint.aspect_ratio());
    _project.exporter.video_export_window().open();
}

} // namespace Lab
