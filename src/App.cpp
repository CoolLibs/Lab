#include "App.h"
#include <Cool/DebugOptions/TestMessageConsole.h>
#include <Cool/DebugOptions/TestPresets.h>
#include <Cool/ImGui/Fonts.h>
#include <Cool/ImGui/icon_fmt.h>
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>
#include <Cool/Time/ClockU.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Variables/TestVariables.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h>
#include <cmd/imgui.hpp>
#include <stringify/stringify.hpp>
#include "CommandCore/command_to_string.h"
#include "Commands/Command_OpenImageExporter.h"
#include "Commands/Command_OpenVideoExporter.h"
#include "Cool/Gpu/TextureLibrary.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/Log/Message.h"
#include "Cool/Nodes/ImNodes_StyleEditor.h"
#include "Debug/DebugOptions.h"
#include "Dependencies/Camera2DManager.h"
#include "Dump/gen_dump_string.h"
#include "Menus/about_menu.h"
#include "Module_is0/Module_is0.h"
#include "UI/imgui_show.h"
#include "imgui.h"

namespace Lab {

App::App(Cool::WindowManager& windows)
    : _camera_manager{_variable_registries.of<Cool::Variable<Cool::Camera>>().create_shared({})}
    , _main_window{windows.main_window()}
    , _nodes_view{_views.make_view(Cool::icon_fmt("View", ICOMOON_IMAGE))}
    // , _custom_shader_view{_views.make_view("View | Custom Shader")}
    , _nodes_module{std::make_unique<Module_Nodes>(dirty_flag_factory(), input_factory())}
// , _custom_shader_module{std::make_unique<Module_CustomShader>(dirty_flag_factory(), input_factory())}
{
    _camera_manager.hook_events(_nodes_view.view.mouse_events(), _variable_registries, command_executor());
    hook_camera2D_events(
        _nodes_view.view.mouse_events(),
        _camera2D.value(),
        [this]() { trigger_rerender(); },
        [this]() { auto const sz =_nodes_view.view.size(); return sz ? static_cast<float>(sz->height()) : 1.f; },
        [this]() { auto const sz =_nodes_view.view.size(); return sz ? img::SizeU::aspect_ratio(*sz) : 1.f; },
        [this]() { return _is_camera_2D_locked_in_view; }
    );
    // _camera_manager.hook_events(_custom_shader_view.view.mouse_events(), _variable_registries, command_executor());
    // serv::init([](std::string_view request) {
    //     Cool::Log::Debug::info("Scripting", "{}", request);
    // });
    _clock.pause();
}

App::~App()
{
    // serv::shut_down();
}

void App::compile_all_is0_nodes()
{
    // for (const auto& node_template : _nodes_module->nodes_templates())
    // {
    //     _nodes_module->remove_all_nodes();
    //     Cool::Log::ToUser::info("Test is0 Node", node_template.name);
    //     _nodes_module->add_node(NodeFactoryU::node_from_template(node_template));
    //     _nodes_module->recompile(update_context(), true);
    // }
    // _nodes_module->remove_all_nodes();
}

void App::set_everybody_dirty()
{
    std::unique_lock lock{_dirty_registry.mutex()};
    for (auto& [_, is_dirty] : _dirty_registry)
        is_dirty.is_dirty = true;
}

void App::update()
{
    if (_is_first_frame)
    {
        _is_first_frame = false;
        set_everybody_dirty();
    }

    Cool::user_settings().color_themes.update();

    if (inputs_are_allowed()) // Must update() before we render() to make sure the modules are ready (e.g. Nodes need to parse the definitions of the nodes from files)
    {
        _nodes_module->update(update_context());
        // _custom_shader_module->update(update_context());
        check_inputs();
    }

    if (!_exporter.is_exporting())
    {
        _clock.update();
        for (auto& view : _views)
        {
            view.update_size(_view_constraint);
        }
        polaroid().render(_clock.time());
    }
    else
    {
        trigger_rerender();
        _exporter.update(polaroid());
    }

    if (_last_time != _clock.time())
    {
        _last_time = _clock.time();

        trigger_rerender();
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
        auto const path   = Cool::Path::root() / "info_dump.txt";
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

void App::trigger_rerender()
{
    set_dirty_flag()(_nodes_module->dirty_flag());
}

auto App::all_inputs() -> Cool::AllInputRefsToConst
{
    // auto vec  = _custom_shader_module->all_inputs();
    auto vec2 = _nodes_module->all_inputs();
    // for (const auto& x : vec2)
    // {
    //     vec.push_back(x);
    // }
    // return vec;
    return vec2;
}

Cool::Polaroid App::polaroid()
{
    return {
        .render_target = _nodes_view.render_target,
        .render_fn     = [this](Cool::RenderTarget& render_target, float time) {
            render(render_target, time);
        }};
}

auto App::inputs_are_allowed() const -> bool
{
    return !_exporter.is_exporting();
}

auto App::wants_to_show_menu_bar() const -> bool
{
    return !_exporter.is_exporting();
}

static void imgui_window_console()
{
    Cool::Log::ToUser::console().imgui_window();
#if DEBUG
    Cool::Log::Debug::console().imgui_window();
#endif
}

static void imgui_window_exporter(Cool::Exporter& exporter, Cool::Polaroid polaroid, float time)
{
    exporter.imgui_windows(polaroid, time);
}

void App::render_one_module(Module& some_module, Cool::RenderTarget& render_target, float time)
{
    render_target.render([&]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        const auto aspect_ratio = img::SizeU::aspect_ratio(render_target.desired_size());
        some_module.do_rendering(
            {
                input_provider(aspect_ratio, static_cast<float>(render_target.desired_size().height()), time, _camera2D.value().transform_matrix()),
                input_factory(),
                is_dirty__functor(),
                set_clean__functor(),
                _variable_registries,
            },
            update_context()
        );
    });
    if (DebugOptions::log_when_rendering())
        Cool::Log::ToUser::info(some_module.name() + " Rendering", "Rendered");
}

void App::render_nodes(Cool::RenderTarget& render_target, float time, img::Size size)
{
    if (render_target.needs_resizing())
        trigger_rerender();

    if (!_nodes_module->is_dirty(is_dirty__functor()))
        return;

    render_target.set_size(size);
    render_one_module(*_nodes_module, render_target, time);
}

// void App::render_custom_shader(Cool::RenderTarget& render_target, float time)
// {
// _custom_shader_module->set_image_in_shader("_image", 0, _nodes_view.render_target.get().texture_id());
// render_one_module(*_custom_shader_module, render_target, time);
// }

void App::render(Cool::RenderTarget& render_target, float time)
{
    render_nodes(_nodes_view.render_target, time, render_target.desired_size());
    // render_custom_shader(render_target, time);
}

void App::imgui_commands_and_registries_debug_windows()
{
    const auto the_ui = ui();
    the_ui.window({.name = "Registry of vec3"}, [&]() {
        imgui_show(_variable_registries.of<Cool::Variable<glm::vec3>>());
    });
    the_ui.window({.name = "Registry of float"}, [&]() {
        imgui_show(_variable_registries.of<Cool::Variable<float>>());
    });
    the_ui.window({.name = "Registry of int"}, [&]() {
        imgui_show(_variable_registries.of<Cool::Variable<int>>());
    });
    the_ui.window({.name = "Registry of Camera"}, [&]() {
        imgui_show(_variable_registries.of<Cool::Variable<Cool::Camera>>());
    });
    the_ui.window({.name = "Registry of DirtyFlag"}, [&]() {
        imgui_show(_dirty_registry);
    });
    the_ui.window({.name = "History"}, [&]() {
        _history.imgui_show([](const ReversibleCommand& command) {
            return command_to_string(command);
        });
    });
    the_ui.window({.name = "Command Logger"}, [&]() {
        _command_logger.imgui_show();
    });
}

void App::cameras_window()
{
    static constexpr auto help_text = "When enabled, prevents you from changing your camera by clicking in the View. This can be useful when working with both 2D and 3D nodes: you don't want both the 2D and 3D cameras active at the same time.";

    ImGui::PushID("##2D");
    ImGui::PushFont(Cool::Font::bold());
    ImGui::SeparatorText("2D Camera");
    ImGui::PopFont();
    Cool::ImGuiExtras::toggle("Locked in view", &_is_camera_2D_locked_in_view);
    ImGui::SameLine();
    Cool::ImGuiExtras::help_marker(help_text);
    if (imgui_widget(_camera2D))
        trigger_rerender();
    ImGui::PopID();

    ImGui::NewLine();

    ImGui::PushID("##3D");
    ImGui::PushFont(Cool::Font::bold());
    ImGui::SeparatorText("3D Camera");
    ImGui::PopFont();
    Cool::ImGuiExtras::toggle("Locked in view", &_camera_manager.is_locked_in_view());
    ImGui::SameLine();
    Cool::ImGuiExtras::help_marker(help_text);
    _camera_manager.imgui(_variable_registries, command_executor());
    ImGui::PopID();
}

void App::imgui_windows()
{
    bool const is_exporting = _exporter.is_exporting();
    {
        if (!_was_exporting_during_previous_frame && is_exporting)
            save_windows_state(); // Save normal state before making the View fullscreen.
        if (_was_exporting_during_previous_frame && !is_exporting)
            restore_windows_state();
        _was_exporting_during_previous_frame = is_exporting;
    }

    _nodes_view.imgui_window(is_exporting /* View in fullscreen while we are exporting */);

    imgui_window_exporter(_exporter, polaroid(), _clock.time());

    imgui_window_console();

    if (inputs_are_allowed())
    {
        const auto the_ui = ui();
        _nodes_module->imgui_windows(the_ui);
        // _custom_shader_module->imgui_windows(the_ui);
        // Time
        ImGui::Begin(Cool::icon_fmt("Time", ICOMOON_STOPWATCH).c_str());
        Cool::ClockU::imgui_timeline(_clock);
        ImGui::End();
        // Cameras
        ImGui::Begin(Cool::icon_fmt("Cameras", ICOMOON_CAMERA).c_str());
        cameras_window();
        ImGui::End();

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
            _nodes_module->debug_show_nodes_and_links_registries_windows(ui());
        }

        Cool::DebugOptions::texture_library_debug_view([&] {
            Cool::TextureLibrary::instance().imgui_debug_view();
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

        Cool::DebugOptions::color_themes_advanced_config_window([&]() {
            Cool::user_settings().color_themes.imgui_advanced_config();
        });

        Cool::DebugOptions::color_themes_editor([&]() {
            Cool::user_settings().color_themes.imgui_basic_theme_editor();
        });

        DebugOptions::imnodes_color_theme_window([&]() {
            _imnodes_style.widget();
        });
    }
}

void App::view_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("View", ICOMOON_IMAGE, true).c_str()))
    {
        if (_view_constraint.imgui())
        {
            // render_impl(_view.render_target, *_current_module, _clock.time());
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
        _exporter.imgui_menu_items({
            .open_image_exporter = [&]() { command_executor().execute(Command_OpenImageExporter{}); },
            .open_video_exporter = [&]() { command_executor().execute(Command_OpenVideoExporter{}); },
        });
        ImGui::EndMenu();
    }
}

void App::settings_menu()
{
    if (ImGui::BeginMenu(Cool::icon_fmt("Settings", ICOMOON_COG, true).c_str()))
    {
        Cool::user_settings().imgui();
        ImGui::Separator();

        ImGui::SeparatorText("History");
        _history.imgui_max_size();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::Separator();

        _history.imgui_max_saved_size();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::Separator();

        ImGui::SeparatorText("Color Theme");
        Cool::user_settings().color_themes.imgui_theme_picker();

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
    view_menu();
    // windows_menu();/// This menu might make sense if we have several views one day, but for now it just creates a menu for no reason
    export_menu();
    settings_menu();

    ImGui::SetCursorPosX( // HACK while waiting for ImGui to support right-to-left layout. See issue https://github.com/ocornut/imgui/issues/5875
        ImGui::GetWindowSize().x
        - ImGui::CalcTextSize("AboutDebug").x
        - 3.f * ImGui::GetStyle().ItemSpacing.x
        - ImGui::GetStyle().WindowPadding.x
    );
    about_menu();
    debug_menu();
}

void App::check_inputs()
{
    if (!ImGui::GetIO().WantTextInput)
    {
        check_inputs__history();
        check_inputs__export_windows();
        check_inputs__timeline();
    }
}

void App::check_inputs__history()
{
    auto        exec = reversible_command_executor_without_history();
    const auto& io   = ImGui::GetIO();

    // Undo
    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Z))
    {
        _history.move_backward(exec);
    }

    // Redo
    if ((io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y))
        || (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_Z)))
    {
        _history.move_forward(exec);
    }
}

void App::check_inputs__export_windows()
{
    auto const& io = ImGui::GetIO();

    if (ImGui::IsKeyReleased(ImGuiKey_S) && io.KeyCtrl)
        command_executor().execute(Command_OpenImageExporter{});
    if (ImGui::IsKeyReleased(ImGuiKey_E) && io.KeyCtrl)
        command_executor().execute(Command_OpenVideoExporter{});
}

void App::check_inputs__timeline()
{
    if (ImGui::IsKeyReleased(ImGuiKey_Space))
    {
        _clock.toggle_play_pause();
    }
}

void App::on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views)
    {
        view.view.dispatch_mouse_button_event(view_event(event, view));
    }
}

void App::on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views)
    {
        view.view.dispatch_mouse_scroll_event(view_event(event, view));
    }
}

void App::on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views)
    {
        view.view.dispatch_mouse_move_event(view_event(event, view));
    }
}

void App::open_image_exporter()
{
    _exporter.maybe_set_aspect_ratio(_view_constraint.aspect_ratio());
    _exporter.image_export_window().open();
}

void App::open_video_exporter()
{
    _exporter.maybe_set_aspect_ratio(_view_constraint.aspect_ratio());
    _exporter.video_export_window().open();
}

} // namespace Lab