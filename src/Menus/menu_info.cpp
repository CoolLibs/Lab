#include "menu_info.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Path/Path.h>
#include <Cool/Utils/Version.h>

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

        static constexpr auto show_version = [](const char* text, int version) {
            line(
                [text] { ImGui::TextDisabled("%s", text); },
                [version] { ImGui::Text("%d.%d", Cool::major_version(version), Cool::minor_version(version)); }
            );
        };

        if (ImGui::BeginTable("table121", 2, flags))
        {
            line(
                [] { ImGui::TextDisabled("Website"); },
                [] { Cool::ImGuiExtras::link("https://coollab-art.com/"); }
            );

            line(
                [] { ImGui::TextDisabled("CoolLab version"); },
                [] { ImGui::Text("beta-0"); }
            );

#if COOL_OPENGL
            show_version("OpenGL version", COOL_OPENGL_VERSION);
#elif COOL_VULKAN
            show_version("Vulkan version", COOL_VULKAN_VERSION);
#else
#error Unknown GPU API
#endif
            line(
                [] { ImGui::TextDisabled("Working directory"); },
                [] { ImGui::TextUnformatted(Cool::Path::root().string().c_str()); }
            );

            line(
                [] { ImGui::TextDisabled("Cool directory"); },
                [] { ImGui::TextUnformatted(Cool::Path::cool_res().string().c_str()); }
            );

            ImGui::EndTable();
        }

        ImGui::EndMenu();
    }
}

} // namespace Lab
