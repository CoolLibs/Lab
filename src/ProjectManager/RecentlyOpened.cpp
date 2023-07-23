#include "RecentlyOpened.h"
#include <CommandCore/CommandExecutionContext_Ref.h>
#include <ProjectManager/Command_OpenProject.h>
#include <imgui.h>
#include <filesystem>
#include <optional>

namespace Lab {

RecentlyOpened::RecentlyOpened()
    : _dialog_window{Cool::icon_fmt("Open Recent", ICOMOON_FILE_TEXT2), Cool::ImGuiWindowConfig{.is_modal = true}}
{}

void RecentlyOpened::on_project_opened(std::filesystem::path path)
{
    path = std::filesystem::weakly_canonical(path);
    std::erase(_list.underlying_container(), path);
    _list.push_front(std::move(path));
}

auto RecentlyOpened::most_recent_path() const -> std::optional<std::filesystem::path>
{
    for (auto const& path : _list)
    {
        if (std::filesystem::exists(path))
            return path;
    }
    return std::nullopt;
}

void RecentlyOpened::imgui_window(CommandExecutionContext_Ref const& ctx)
{
    auto selected_path = std::optional<std::filesystem::path>{};
    _dialog_window.show([&]() {
        bool has_set_keyboard_focus{false};
        for (auto const& path : _list)
        {
            // Find the first path that is not the current path and focus it.
            if (ImGui::IsWindowAppearing()
                && !has_set_keyboard_focus
                && (!ctx.project_path() || path != std::filesystem::weakly_canonical(*ctx.project_path())))
            {
                ImGui::SetKeyboardFocusHere();
                has_set_keyboard_focus = true;
            }
            if (ImGui::Selectable(fmt::format("{} [{}]", path.stem().string(), path.parent_path().string()).c_str()))
            {
                selected_path = path;
            }
        }
    });

    if (selected_path)
    {
        ctx.execute(Command_OpenProject{.path = *selected_path});
        _dialog_window.close();
    }
}

void RecentlyOpened::open_window()
{
    remove_paths_that_dont_exist_anymore();
    _dialog_window.open();
}

void RecentlyOpened::remove_paths_that_dont_exist_anymore()
{
    std::erase_if(_list.underlying_container(), [](std::filesystem::path const& path) {
        return !std::filesystem::exists(path);
    });
}

} // namespace Lab