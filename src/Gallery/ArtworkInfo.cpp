#include "ArtworkInfo.h"
#include <imgui.h>

namespace Lab {

auto ArtworkInfo::imgui() -> bool
{
    bool b = false;
    b |= ImGui::InputText("Title", &title);
    b |= ImGui::InputText("Description", &description);
    return b;
}

} // namespace Lab