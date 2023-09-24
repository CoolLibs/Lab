#pragma once

#include <reg/reg.hpp>
#include <stringify/stringify.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "imgui.h"

namespace Lab {

template<typename T>
void imgui_show(T const& value)
{
    auto const text = Cool::stringify(value);
    ImGui::TextUnformatted(text.c_str());
    bool const clicked = ImGui::IsItemClicked();
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    ImGui::SetItemTooltip("%s", "Copy to clipboard");
    if (clicked)
        ImGui::SetClipboardText(text.c_str());
}

template<typename T>
void imgui_show(Cool::Variable<T> const& variable)
{
    imgui_show(variable.value());
}

struct IsARegistryISwear {};
template<typename Registry>
void imgui_show(Registry const& registry, IsARegistryISwear)
{
    std::shared_lock                 lock{registry.mutex()};
    static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
                                             | ImGuiTableFlags_Resizable
                                             | ImGuiTableFlags_BordersOuter
                                             | ImGuiTableFlags_BordersV
                                             | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("table1", 2, flags))
    {
        for (auto const& [id, value] : registry)
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

template<typename T>
void imgui_show(reg::Registry<T> const& registry)
{
    imgui_show(registry, IsARegistryISwear{});
}
template<typename T>
void imgui_show(reg::RawRegistry<T> const& registry)
{
    imgui_show(registry, IsARegistryISwear{});
}
template<typename T>
void imgui_show(reg::OrderedRegistry<T> const& registry)
{
    imgui_show(registry, IsARegistryISwear{});
}
template<typename T>
void imgui_show(reg::RawOrderedRegistry<T> const& registry)
{
    imgui_show(registry, IsARegistryISwear{});
}

} // namespace Lab
