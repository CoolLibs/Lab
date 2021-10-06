#include "App.h"

App::App(Cool::WindowManager& windows)
    : DefaultApp::DefaultApp{windows, [&](Cool::RenderTarget& render_target, float time) { render(render_target, time); }}
{
}

void App::update()
{
    DefaultApp::update();
    if (inputs_are_allowed()) {
        _shader_manager->update();
    }
}

void App::render(Cool::RenderTarget& render_target, float time)
{
#if defined(COOL_VULKAN)
#elif defined(COOL_OPENGL)
    render_target.render([&]() {
        const auto aspect_ratio = ImageSizeU::aspect_ratio(render_target.current_size());
        _camera.apply(aspect_ratio);
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (_shader_manager->is_valid()) {
            _shader_manager->setup_for_rendering(*_camera, 2.f /*TODO*/, aspect_ratio, time);
            _shader_manager->render();
        }
    });
#endif
}

void App::imgui_windows()
{
    DefaultApp::imgui_windows();
    if (inputs_are_allowed()) {
        _shader_manager.ImGui_windows();
    }
}

void App::imgui_menus()
{
    DefaultApp::imgui_menus();
}

void App::on_keyboard_event(const Cool::KeyboardEvent& event)
{
    DefaultApp::on_keyboard_event(event);
}

void App::on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event)
{
    DefaultApp::on_mouse_button(event);
}

void App::on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event)
{
    DefaultApp::on_mouse_scroll(event);
}

void App::on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event)
{
    DefaultApp::on_mouse_move(event);
}