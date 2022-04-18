#include "App.h"
#include "TestModule/TestModule.h"

namespace Lab {

App::App(Cool::WindowManager& windows)
    : DefaultApp::DefaultApp{windows, [&](Cool::RenderTarget& render_target, float time) { render(render_target, time); }}
    , _current_module{std::make_unique<TestModule>(_registries)}
{
    _clock.pause();
#if IS0_TEST_NODES
    for (const auto& node_template : _shader_manager.nodes_templates()) {
        _shader_manager.add_node(NodeFactoryU::node_from_template(node_template));
    }
#endif
}

void App::update()
{
    DefaultApp::update();
    if (inputs_are_allowed()) {
        _current_module->update();
        _shader_manager->update();
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
#if defined(COOL_VULKAN)
#elif defined(COOL_OPENGL)
    if (_current_module->needs_rendering()) {
        render_target.render([&]() {
            const auto aspect_ratio = img::SizeU::aspect_ratio(render_target.current_size());
            _camera.apply(aspect_ratio);
            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT);
            // _shader_manager->setup_for_rendering(*_camera, time);
            // _shader_manager->render();
            _current_module->do_rendering(_registries);
        });
    }
#endif
}

void App::imgui_windows()
{
    DefaultApp::imgui_windows();
    if (inputs_are_allowed()) {
        auto set_dirty = SetDirty{*_current_module};
        auto commands  = ReversibleCommandDispatcher{_registries, set_dirty};
        auto ui        = Ui{_registries, commands};
        _current_module->imgui_windows(ui);
        Ui::window({.name = "Registry of vec3"}, [&]() {
            imgui_show(_registries.of<glm::vec3>());
        });
        // _shader_manager.imgui_windows();
    }
}

void App::imgui_menus()
{
    DefaultApp::imgui_menus();
}

void App::on_keyboard_event(const Cool::KeyboardEvent& event)
{
    DefaultApp::on_keyboard_event(event);
    _shader_manager->on_key_pressed(event);
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

} // namespace Lab