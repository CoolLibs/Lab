#include "App.h"
#include <Cool/Camera/hook_events.h>
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameters/ParametersHistory.h>
#include <Cool/Time/ClockU.h>

App::App(Cool::WindowManager& windows)
    : _main_window{windows.main_window()}
    , _view{_views.make_view("View")}
{
    Cool::CameraU::hook_events(_view.view.mouse_events(), _camera_orbital_controller, _camera);
    _camera_orbital_controller.set_distance_to_orbit_center(15.f);
}

void App::update()
{
    if (!_exporter.is_exporting()) {
        _clock.update();
        _shader_manager->update();
        for (auto& view : _views) {
            view.update_size(_preview_constraint);
        }
        polaroid().render(_clock.time());
    }
    else {
        _exporter.update(polaroid());
    }
}

void App::render(Cool::RenderTarget& render_target, float time)
{
#if defined(COOL_VULKAN)
#elif defined(COOL_OPENGL)
    render_target.render([&]() {
        const auto aspect_ratio = ImageSizeU::aspect_ratio(render_target.current_size());
        _camera_perspective_controller.apply_to(_camera, aspect_ratio);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (_shader_manager->is_valid()) {
            _shader_manager->setup_for_rendering(_camera, _camera_perspective_controller.focal_length(), aspect_ratio, time);
            _shader_manager->render();
        }
    });
#endif
}

Cool::Polaroid App::polaroid()
{
    return {
        .render_target = _view.render_target,
        .render_fn     = [&](Cool::RenderTarget& render_target, float time) {
            render(render_target, time);
        }};
}

bool App::inputs_are_allowed() const
{
    return !_exporter.is_exporting();
}

bool App::wants_to_show_menu_bar() const
{
    return !_exporter.is_exporting();
}

void App::imgui_windows()
{
    // Views
    for (const bool aspect_ratio_is_constrained = _exporter.is_exporting() || // cppcheck-suppress syntaxError // (CppCheck is not yet aware of this C++20 syntax)
                                                  _preview_constraint.wants_to_constrain_aspect_ratio();
         auto& view : _views) {
        view.imgui_window(aspect_ratio_is_constrained);
    }
    // Exporter
    _exporter.imgui_windows(polaroid(), _clock.time());
    //
    if (!_exporter.is_exporting()) {
        // Console
        Log::ToUser::imgui_console_window();
        // Time
        ImGui::Begin("Time");
        Cool::ClockU::imgui_timeline(_clock);
        ImGui::End();
        // Camera
        ImGui::Begin("Camera");
        _camera_orbital_controller.ImGui();
        if (ImGui::Button("Look at the origin")) {
            _camera_orbital_controller.set_orbit_center({0, 0, 0}, _camera);
        }
        _camera_perspective_controller.ImGui();
        ImGui::End();
        // ShaderManager
        _shader_manager.ImGui_windows();
#if defined(DEBUG)
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
}

void App::imgui_menus()
{
    if (ImGui::BeginMenu("Preview")) {
        _preview_constraint.imgui();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Windows")) {
        Cool::Log::ToUser::imgui_toggle_console();
        for (auto& view : _views) {
            view.view.imgui_open_close_checkbox();
        }
#if defined(DEBUG)
        ImGui::Separator();
        ImGui::Checkbox("Debug", &_show_imgui_debug);
#endif
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Export")) {
        _exporter.imgui_menu_items();
        ImGui::EndMenu();
    }
}

void App::on_keyboard_event(const Cool::KeyboardEvent& event)
{
    if (event.action == GLFW_RELEASE) {
        if (Input::matches_char("s", event.key) && event.mods.ctrl()) {
            _exporter.image_export_window().open();
        }
        if (Input::matches_char("e", event.key) && event.mods.ctrl()) {
            _exporter.video_export_window().open();
        }
    }
    if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT) {
        if (Input::matches_char("z", event.key) && event.mods.ctrl()) {
            ParametersHistory::get().move_backward();
        }
        if (Input::matches_char("y", event.key) && event.mods.ctrl()) {
            ParametersHistory::get().move_forward();
        }
    }
}

void App::on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views) {
        view.view.dispatch_mouse_button_event(event, _main_window.glfw());
    }
}

void App::on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views) {
        view.view.dispatch_mouse_scroll_event(event, _main_window.glfw());
    }
}

void App::on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views) {
        view.view.dispatch_mouse_move_event(event, _main_window.glfw());
    }
}