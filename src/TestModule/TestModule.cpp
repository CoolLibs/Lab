#include "TestModule.h"
#include <Cool/Log/ToUser.h>

namespace Lab {

TestModule::TestModule(std::string_view name)
    : _name{name}
{
}

void TestModule::imgui_windows(Ui ui)
{
    Ui::window({.name = _name}, [&]() {
        for (auto& dep : dependencies()) {
            std::visit([&ui](auto&& dep) {
                ui.widget(dep.get().name(), dep.get());
            },
                       dep);
        }
    });
}

void TestModule::render(InputProvider provider)
{
    Cool::Log::ToUser::info(_name, "Re-rendering");
    if (_fullscreen_pipeline.shader()) {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_color", provider(_color));
        _fullscreen_pipeline.shader()->set_uniform("_color2", provider(_color2));
        _fullscreen_pipeline.shader()->set_uniform("_circle_radius", provider(_testFloat));
        _fullscreen_pipeline.shader()->set_uniform("_aspect_ratio", provider(InputSlot_AspectRatio{}));
        _fullscreen_pipeline.draw();
    }
}

} // namespace Lab
