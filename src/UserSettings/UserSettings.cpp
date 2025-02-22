#include "UserSettings.hpp"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Lab {

auto UserSettings::imgui() -> bool
{
    bool b = false;

    Cool::ImGuiExtras::separator_text("Save As");
    b |= imgui_switch_to_new_project_when_saving_as();

    if (b)
        _serializer.save();
    return b;
}

auto UserSettings::imgui_switch_to_new_project_when_saving_as() -> bool
{
    bool const b = Cool::ImGuiExtras::toggle("Switch to the new project when saving as", &switch_to_new_project_when_saving_as);
    Cool::ImGuiExtras::help_marker("When using \"Save as\" you can either keep working on the current project, or switch to the new one you just saved.\nYou will notice the name changing in the window's title bar if you switch to a different project.");
    return b;
}

} // namespace Lab
