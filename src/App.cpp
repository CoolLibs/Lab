#include "App.h"
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <cmd/imgui.hpp>
#include <serv/serv.hpp>
#include "TestModule/TestModule.h"

namespace Lab {

App::App(Cool::WindowManager& windows)
    : DefaultApp::DefaultApp{windows, [&](Cool::RenderTarget& render_target, float time) { render(render_target, time); }}
    , _intId{_registries.create(0)}
    , _current_module{std::make_unique<TestModule>("Test Module")}
    , _current_module2{std::make_unique<TestModule>("Test Module 2")}
    , _view2{_views.make_view("View2")}
{
    _camera.hook_events(_view2.view.mouse_events());
    serv::init([](std::string_view request) {
        Cool::Log::ToUser::info("Scripting", "{}", request);
    });
    _clock.pause();
#if IS0_TEST_NODES
    for (const auto& node_template : _shader_manager.nodes_templates()) {
        _shader_manager.add_node(NodeFactoryU::node_from_template(node_template));
    }
#endif
}

App::~App()
{
    serv::shut_down();
}

void App::render_impl(Cool::RenderTarget& render_target, Module& some_module)
{
    render_target.render([&]() {
        const auto aspect_ratio = img::SizeU::aspect_ratio(render_target.desired_size());
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        // _camera.apply(aspect_ratio);
        // _shader_manager->setup_for_rendering(*_camera, time);
        // _shader_manager->render();
        some_module.do_rendering(input_provider(aspect_ratio));
    });
}

void App::update()
{
    DefaultApp::update();
    if (_view.render_target.needs_resizing()) {
        _current_module->set_dirty();
    }
    if (_view2.render_target.needs_resizing()) {
        _current_module2->set_dirty();
    }
    if (inputs_are_allowed()) {
        _current_module->update();
        _current_module2->update();
        _shader_manager->update();

        if (_current_module2->needs_rendering()) {
            render_impl(_view2.render_target, *_current_module2);
        }
    }
#if IS0_TEST_NODES
    glfwSetWindowShouldClose(_main_window.glfw(), true);
#endif
}

void App::render(Cool::RenderTarget& render_target, float)
{
#if IS0_TEST_NODES
    render_target.set_size({1, 1});
#endif
#if defined(COOL_VULKAN)
#elif defined(COOL_OPENGL)
    if (_current_module->needs_rendering()) {
        render_impl(render_target, *_current_module);
    }
#endif
}

template<typename T>
static auto command_to_string(const ReversibleCommand_SetValue<T> command) -> std::string
{
    return "Set " + reg::to_string(command.id) + " to " + value_to_string(command.value);
}

void App::imgui_windows()
{
    DefaultApp::imgui_windows();
    if (inputs_are_allowed()) {
        _current_module->imgui_windows(ui());
        _current_module2->imgui_windows(ui());
        Ui::window({.name = "Registry of vec3"}, [&]() {
            imgui_show(_registries.of<glm::vec3>());
        });
        Ui::window({.name = "Registry of float"}, [&]() {
            imgui_show(_registries.of<float>());
        });
        Ui::window({.name = "Registry of int"}, [&]() {
            imgui_show(_registries.of<int>());
        });
        Ui::window({.name = "History"}, [&]() {
            _history.imgui_show([](const ReversibleCommand& command) {
                return std::visit([](const auto& cmd) { return command_to_string(cmd); }, command);
            });
        });
        Ui::window({.name = "TMP"}, [&]() {
            static int n = 1;
            ImGui::InputInt("Value", &n);
            if (ImGui::Button("Set")) {
                commands_dispatcher().dispatch(Command_SetValue<int>{_intId, n});
                commands_dispatcher().dispatch(Command_FinishedEditingValue{});
                n++;
            }
        });
        // _shader_manager.imgui_windows();
    }
}

void App::menu_preview()
{
    if (ImGui::BeginMenu("Preview")) {
        if (_preview_constraint.imgui()) {
            render_impl(_view.render_target, *_current_module);
            render_impl(_view2.render_target, *_current_module2);
        }
        ImGui::EndMenu();
    }
}

void App::menu_windows() { DefaultApp::menu_windows(); }
void App::menu_export() { DefaultApp::menu_export(); }

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
    DefaultApp::on_keyboard_event(event);
    _shader_manager->on_key_pressed(event);
    if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT) {
        auto exec = reversible_commands_executor();
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