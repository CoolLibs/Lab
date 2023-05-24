#include "AuthorInfo.h"
#include <imgui.h>
#include "Cool/ImGui/ImGuiExtras.h"

namespace Lab {

auto AuthorInfo::imgui() -> bool
{
    bool b = false;
    b |= ImGui::InputText("Your name", &name);
    b |= ImGui::InputText("Your link", &link);
    Cool::ImGuiExtras::help_marker("An optional link for people to find you or your work: it can be your portfolio, linktree, Instagram, etc.");
    return b;
}

} // namespace Lab