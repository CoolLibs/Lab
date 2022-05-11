#include "TestModule.h"
#include <Cool/Log/ToUser.h>

namespace Lab {

TestModule::TestModule(std::string_view name, Registries& registries)
    : _name{name}
{
}

void TestModule::imgui_windows(Ui ui)
{
    Ui::window({.name = _name}, [&]() {
        ui.widget("Color", _color);
    });
}

void TestModule::render(InputProvider provider)
{
    Cool::Log::ToUser::info(_name, "Re-rendering");
    if (_fullscreen_pipeline.shader()) {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_color", provider(_color));
        _fullscreen_pipeline.draw();
    }
}

} // namespace Lab
