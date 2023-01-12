#include "App.h"
#include <Cool/DebugOptions/TestMessageConsole.h>
#include <Cool/DebugOptions/TestPresets.h>
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParametersHistory.h>
#include <Cool/Path/Path.h>
#include <Cool/Time/ClockU.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Variables/TestVariables.h>
#include <cmd/imgui.hpp>
#include <serv/serv.hpp>
#include <stringify/stringify.hpp>
#include "CommandCore/command_to_string.h"
#include "Commands/Command_OpenImageExporter.h"
#include "Commands/Command_OpenVideoExporter.h"
#include "Debug/DebugOptions.h"
#include "Debug/compile_all_custom_shaders.h"
#include "Menus/menu_info.h"
#include "Module_CustomShader/Module_CustomShader.h"
#include "Module_is0/Module_is0.h"
#include "UI/imgui_show.h"

namespace Lab {

App::App(Cool::WindowManager& windows)
    : _camera_manager{_variable_registries.of<Cool::Variable<Cool::Camera>>().create({})}
    , _main_window{windows.main_window()}
    , _nodes_view{_views.make_view("View | Nodes")}
    // , _custom_shader_view{_views.make_view("View | Custom Shader")}
    , _nodes_module{std::make_unique<Module_Nodes>(dirty_flag_factory())}
// , _custom_shader_module{std::make_unique<Module_CustomShader>(dirty_flag_factory(), input_factory())}
{
    _camera_manager.hook_events(_nodes_view.view.mouse_events(), _variable_registries, command_executor());
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
    //     Cool::Log::Debug::info("Test is0 Node", node_template.name);
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
            view.update_size(_preview_constraint);
        }
        polaroid().render(_clock.time());
    }
    else
    {
        set_dirty_flag()(_nodes_module->dirty_flag());
        _exporter.update(polaroid());
    }

    if (_last_time != _clock.time())
    {
        _last_time = _clock.time();

        set_dirty_flag()(_nodes_module->dirty_flag());
        // set_dirty_flag()(_custom_shader_module->dirty_flag());
    }
    // if (_custom_shader_view.render_target.needs_resizing())
    // {
    // set_dirty_flag()(_custom_shader_module->dirty_flag());
    // }
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
                input_provider(aspect_ratio, static_cast<float>(render_target.desired_size().height()), time),
                input_factory(),
                input_destructor(),
                is_dirty__functor(),
                set_clean__functor(),
                _variable_registries,
            },
            update_context()
        );
    });
#if DEBUG
    if (DebugOptions::log_when_rendering())
        Cool::Log::Debug::info(some_module.name() + " Rendering", "Rendered");
#endif
}

void App::render_nodes(Cool::RenderTarget& render_target, float time, img::Size size)
{
    if (render_target.needs_resizing())
        set_dirty_flag()(_nodes_module->dirty_flag());

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

void App::imgui_windows()
{
    _nodes_view.imgui_window();
    // _custom_shader_view.imgui_window();

    imgui_window_exporter(_exporter, polaroid(), _clock.time());

    imgui_window_console();

    if (inputs_are_allowed())
    {
        const auto the_ui = ui();
        _nodes_module->imgui_windows(the_ui);
        // _custom_shader_module->imgui_windows(the_ui);
        // Time
        ImGui::Begin("Time");
        Cool::ClockU::imgui_timeline(_clock);
        ImGui::End();
        // Camera
        ImGui::Begin("Camera");
        _camera_manager.imgui(_variable_registries, command_executor());
        ImGui::End();
#if DEBUG
        DebugOptions::show_framerate_window([&] {
            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            _main_window.imgui_cap_framerate();
        });
        if (DebugOptions::show_imgui_demo_window())                         // Show the big demo window (Most of the sample code is
        {                                                                   // in ImGui::ShowDemoWindow()! You can browse its code
            ImGui::ShowDemoWindow(&DebugOptions::show_imgui_demo_window()); // to learn more about Dear ImGui!).
        }
        if (DebugOptions::show_commands_and_registries_debug_windows())
        {
            imgui_commands_and_registries_debug_windows();
        }
        DebugOptions::test_all_variable_widgets__window(&Cool::test_variables);
        DebugOptions::test_shaders_compilation__window([&]() {
            const auto compile_custom_shaders = [&]() {
                compile_all_custom_shaders(
                    input_provider(2.f, 1.f, 0.f),
                    input_factory(),
                    input_destructor(),
                    update_context()
                );
            };
            if (ImGui::Button("Compile everything"))
            {
                Cool::Log::Debug::console().clear();
                compile_custom_shaders();
                compile_all_is0_nodes();
            }
            ImGui::Separator();
            if (ImGui::Button("Compile all Custom Shaders"))
            {
                Cool::Log::Debug::console().clear();
                compile_custom_shaders();
            }
            if (ImGui::Button("Compile all is0 Nodes"))
            {
                Cool::Log::Debug::console().clear();
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
#endif // DEBUG
    }
}

void App::menu_preview()
{
    if (ImGui::BeginMenu("Preview"))
    {
        if (_preview_constraint.imgui())
        {
            // render_impl(_view.render_target, *_current_module, _clock.time());
        }
        ImGui::EndMenu();
    }
}

void App::menu_windows()
{
    if (ImGui::BeginMenu("Windows"))
    {
        for (auto& view : _views)
        {
            view.view.imgui_open_close_checkbox();
        }
        ImGui::EndMenu();
    }
}

void App::menu_export()
{
    if (ImGui::BeginMenu("Export"))
    {
        _exporter.imgui_menu_items({
            .open_image_exporter = [&]() { command_executor().execute(Command_OpenImageExporter{}); },
            .open_video_exporter = [&]() { command_executor().execute(Command_OpenVideoExporter{}); },
        });
        ImGui::EndMenu();
    }
}

void App::menu_settings()
{
    if (ImGui::BeginMenu("Settings"))
    {
        Cool::user_settings().imgui();
        ImGui::Separator();

        _history.imgui_max_size();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::Separator();

        _history.imgui_max_saved_size();
        ImGui::Separator();
        ImGui::Separator();
        ImGui::Separator();

        _theme_manager.imgui();

        ImGui::EndMenu();
    }
}

void App::menu_debug()
{
#if DEBUG
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
#endif
}

void App::imgui_menus()
{
    menu_preview();
    menu_windows();
    menu_export();
    menu_settings();
    menu_info();
    menu_debug();
}

void App::check_inputs()
{
    if (!ImGui::GetIO().WantTextInput)
    {
        check_inputs__history();
        check_inputs__export_windows();
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
        Cool::ParametersHistory::get().move_backward();
    }

    // Redo
    if ((io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_Y))
        || (io.KeyCtrl && io.KeyShift && ImGui::IsKeyPressed(ImGuiKey_Z)))
    {
        _history.move_forward(exec);
        Cool::ParametersHistory::get().move_forward();
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
    _exporter.maybe_set_aspect_ratio(_preview_constraint.aspect_ratio());
    _exporter.image_export_window().open();
}

void App::open_video_exporter()
{
    _exporter.maybe_set_aspect_ratio(_preview_constraint.aspect_ratio());
    _exporter.video_export_window().open();
}

} // namespace Lab