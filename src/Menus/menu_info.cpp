#include "menu_info.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include "Dump/coollab_version.h"

namespace Lab {

void menu_info()
{
    if (ImGui::BeginMenu("Info"))
    {
        static constexpr ImGuiTableFlags flags = ImGuiTableFlags_BordersOuter
                                                 | ImGuiTableFlags_BordersV
                                                 | ImGuiTableFlags_BordersH
                                                 | ImGuiTableFlags_SizingFixedFit;

        static constexpr auto line = [](auto&& func1, auto&& func2) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            func1();
            ImGui::TableSetColumnIndex(1);
            func2();
        };

        if (ImGui::BeginTable("table121", 2, flags))
        {
            line(
                [] { ImGui::TextDisabled("Website"); },
                [] { Cool::ImGuiExtras::link("https://coollab-art.com/"); }
            );

            line(
                [] { ImGui::TextDisabled("CoolLab version"); },
                [] { ImGui::TextUnformatted(coollab_version().c_str()); }
            );

            ImGui::EndTable();
        }

        ImGui::EndMenu();
    }
}

} // namespace Lab
