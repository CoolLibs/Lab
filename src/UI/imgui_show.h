#pragma once

#include <reg/reg.hpp>
#include <stringify/stringify.hpp>

namespace Lab {

template<typename T>
void imgui_show(const Cool::Variable<T>& variable)
{
    ImGui::TextUnformatted(Cool::stringify(variable.value).c_str());
}

template<typename T>
void imgui_show(const T& value)
{
    ImGui::TextUnformatted(Cool::stringify(value).c_str());
}

template<typename T>
void imgui_show(const reg::Registry<T>& registry)
{
    std::shared_lock                 lock{registry.mutex()};
    static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
                                             | ImGuiTableFlags_Resizable
                                             | ImGuiTableFlags_BordersOuter
                                             | ImGuiTableFlags_BordersV
                                             | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("table1", 2, flags))
    {
        for (const auto& [id, value] : registry)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            imgui_show(value);
            ImGui::TableSetColumnIndex(1);
            imgui_show(id);
        }
        ImGui::EndTable();
    }
}

} // namespace Lab
