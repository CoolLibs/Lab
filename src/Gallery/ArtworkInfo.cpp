#include "ArtworkInfo.h"
#include <imgui.h>
#include "Cool/ImGui/ImGuiExtras.h"

namespace Lab {

auto ArtworkInfo::imgui() -> bool
{
    bool b = false;
    b |= ImGui::InputText("Title", &title);
    b |= ImGui::InputText("Description", &description);
    b |= ImGui::InputText("Your name", &author_name);
    b |= ImGui::InputText("Your link", &author_link);
    ImGui::SameLine();
    Cool::ImGuiExtras::help_marker("An optional link for people to find you or your work: it can be your portfolio, linktree, Instagram, etc.");
    return b;
}

} // namespace Lab