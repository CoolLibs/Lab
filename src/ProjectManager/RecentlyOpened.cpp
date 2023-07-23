#include "RecentlyOpened.h"
#include <imgui.h>

namespace Lab {

void RecentlyOpened::on_project_opened(std::filesystem::path const& path)
{
    _list.push_back(path);
}

auto RecentlyOpened::get_list() -> std::vector<std::filesystem::path> const&
{
    return _list;
}

auto RecentlyOpened::imgui() -> std::optional<std::filesystem::path>
{
    auto res = std::optional<std::filesystem::path>{};
    for (auto const& path : _list)
    {
        if (ImGui::Selectable(fmt::format("{}", path).c_str()))
        {
            res = path;
        }
    }
    return res;
}

} // namespace Lab