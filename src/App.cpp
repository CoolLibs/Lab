#include "App.h"
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParametersHistory.h>
#include <Cool/Time/ClockU.h>
#include <cmd/imgui.hpp>
#include <serv/serv.hpp>
#include <stringify/stringify.hpp>
#include "Commands/command_to_string.h"
#include "Module_CustomShader/Module_CustomShader.h"
#include "UI/imgui_show.h"

namespace Lab {

App::App(Cool::WindowManager& windows)
    : _main_window{windows.main_window()}
    , _camera_manager{_variable_registries.of<Cool::Camera>().create({})}
    , _view{_views.make_view("View")}
    , _current_module{std::make_unique<Module_CustomShader>(dirty_flag_factory())}
{
    _camera_manager.hook_events(_view.view.mouse_events(), _variable_registries, commands_dispatcher());
    // serv::init([](std::string_view request) {
    //     Cool::Log::ToUser::info("Scripting", "{}", request);
    // });
    _clock.pause();
#if IS0_TEST_NODES
    for (const auto& node_template : _shader_manager.nodes_templates()) {
        _shader_manager.add_node(NodeFactoryU::node_from_template(node_template));
    }
#endif
}

App::~App()
{
    // serv::shut_down();
}

void App::render_impl(Cool::RenderTarget& render_target, Module& some_module, float time)
{
    render_target.render([&]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        const auto aspect_ratio = img::SizeU::aspect_ratio(render_target.desired_size());
        some_module.do_rendering({input_provider(aspect_ratio, time), input_slot_destructor(), dirty_manager()});
    });
}

void App::update()
{
    if (!_exporter.is_exporting()) {
        _clock.update();
        for (auto& view : _views) {
            view.update_size(_preview_constraint);
        }
        polaroid().render(_clock.time());
    }
    else {
        _exporter.update(polaroid());
    }

    if (_last_time != _clock.time()) {
        _last_time = _clock.time();

        set_dirty_flag()(_current_module->dirty_flag());
    }
    if (_view.render_target.needs_resizing()) {
        set_dirty_flag()(_current_module->dirty_flag());
    }
    if (inputs_are_allowed()) {
        _current_module->update();
    }
#if IS0_TEST_NODES
    glfwSetWindowShouldClose(_main_window.glfw(), true);
#endif
}

void App::render(Cool::RenderTarget& render_target, float time)
{
#if IS0_TEST_NODES
    render_target.set_size({1, 1});
#endif
    if (_current_module->is_dirty(dirty_manager())) {
        render_impl(render_target, *_current_module, time);
    }
}

auto App::all_input_slots() -> AllInputSlots
{
    auto vec = _current_module->all_input_slots();
    // auto vec2 = _current_module2->all_input_slots();
    // for (const auto& x : vec2) {
    //     vec.push_back(x);
    // }
    return vec;
}

Cool::Polaroid App::polaroid()
{
    return {
        .render_target = _view.render_target,
        .render_fn     = [this](Cool::RenderTarget& render_target, float time) { render(render_target, time); }};
}

auto App::aspect_ratio_is_constrained() const -> bool
{
    return _exporter.is_exporting() ||
           _preview_constraint.wants_to_constrain_aspect_ratio();
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
    Cool::Log::ToUser::imgui_console_window();
}

static void imgui_window_views(Cool::RenderableViewManager& views, bool aspect_ratio_is_constrained)
{
    for (auto& view : views) {
        view.imgui_window(aspect_ratio_is_constrained);
    }
}

static void imgui_window_exporter(Cool::Exporter& exporter, Cool::Polaroid polaroid, float time)
{
    exporter.imgui_windows(polaroid, time);
}

void App::imgui_windows()
{
    imgui_window_views(_views, aspect_ratio_is_constrained());
    imgui_window_exporter(_exporter, polaroid(), _clock.time());
    if (inputs_are_allowed()) {
        // Console
        imgui_window_console();
        // Time
        ImGui::Begin("Time");
        Cool::ClockU::imgui_timeline(_clock);
        ImGui::End();
        // Camera
        ImGui::Begin("Camera");
        _camera_manager.imgui(_variable_registries, commands_dispatcher());
        ImGui::End();
#if DEBUG
        if (_show_imgui_debug) {
            ImGui::Begin("Debug", &_show_imgui_debug);
            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            _main_window.imgui_cap_framerate();
            ImGui::Checkbox("Show Demo Window", &_show_imgui_demo);
            ImGui::End();
        }
        if (_show_imgui_demo) { // Show the big demo window (Most of the sample code is
                                // in ImGui::ShowDemoWindow()! You can browse its code
                                // to learn more about Dear ImGui!).
            ImGui::ShowDemoWindow(&_show_imgui_demo);
        }
#endif
    }

    if (inputs_are_allowed()) {
        _current_module->imgui_windows(ui());
        Ui::window({.name = "Registry of vec3"}, [&]() {
            imgui_show(_variable_registries.of<glm::vec3>());
        });
        Ui::window({.name = "Registry of float"}, [&]() {
            imgui_show(_variable_registries.of<float>());
        });
        Ui::window({.name = "Registry of int"}, [&]() {
            imgui_show(_variable_registries.of<int>());
        });
        Ui::window({.name = "Registry of Camera"}, [&]() {
            imgui_show(_variable_registries.of<Cool::Camera>());
        });
        Ui::window({.name = "Registry of DirtyFlag"}, [&]() {
            imgui_show(_dirty_registry);
        });
        Ui::window({.name = "History"}, [&]() {
            _history.imgui_show([](const ReversibleCommand& command) {
                return std::visit([](const auto& cmd) { return command_to_string(cmd); }, command);
            });
        });
        // _shader_manager.imgui_windows();
    }
}

void App::menu_preview()
{
    if (ImGui::BeginMenu("Preview")) {
        if (_preview_constraint.imgui()) {
            render_impl(_view.render_target, *_current_module, _clock.time());
        }
        ImGui::EndMenu();
    }
}

void App::menu_windows()
{
    if (ImGui::BeginMenu("Windows")) {
        Cool::Log::ToUser::imgui_toggle_console();
        for (auto& view : _views) {
            view.view.imgui_open_close_checkbox();
        }
#if DEBUG
        ImGui::Separator();
        ImGui::Checkbox("Debug", &_show_imgui_debug);
#endif
        ImGui::EndMenu();
    }
}

void App::menu_export()
{
    if (ImGui::BeginMenu("Export")) {
        _exporter.imgui_menu_items();
        ImGui::EndMenu();
    }
}

void App::menu_settings()
{
    if (ImGui::BeginMenu("Settings")) {
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

void App::imgui_menus()
{
    menu_preview();
    menu_windows();
    menu_export();
    menu_settings();
}

void App::on_keyboard_event(const Cool::KeyboardEvent& event)
{
    if (event.action == GLFW_RELEASE) {
        if (Cool::Input::matches_char("s", event.key) && event.mods.ctrl()) {
            _exporter.image_export_window().open();
        }
        if (Cool::Input::matches_char("e", event.key) && event.mods.ctrl()) {
            _exporter.video_export_window().open();
        }
    }
    if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT) {
        if (Cool::Input::matches_char("z", event.key) && event.mods.ctrl()) {
            Cool::ParametersHistory::get().move_backward();
        }
        if (Cool::Input::matches_char("y", event.key) && event.mods.ctrl()) {
            Cool::ParametersHistory::get().move_forward();
        }
    }
    if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT) {
        auto exec = reversible_command_executor();
        if (Cool::Input::matches_char("z", event.key) && event.mods.ctrl()) {
            _history.move_backward(exec);
        }
        if (Cool::Input::matches_char("y", event.key) && event.mods.ctrl()) {
            _history.move_forward(exec);
        }
    }
}

void App::on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views) {
        view.view.dispatch_mouse_button_event(view_event(event, view));
    }
}

void App::on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views) {
        view.view.dispatch_mouse_scroll_event(view_event(event, view));
    }
}

void App::on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views) {
        view.view.dispatch_mouse_move_event(view_event(event, view));
    }
}

} // namespace Lab