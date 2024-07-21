#pragma once
#include <Cool/Containers/CircularBuffer.h>
#include <filesystem>
#include <optional>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "Cool/ImGui/ImGuiWindow.h"

namespace Lab {

class RecentlyOpened {
public:
    RecentlyOpened();

    void on_project_opened(std::filesystem::path);
    auto most_recent_path() const -> std::optional<std::filesystem::path>;
    void imgui_window(CommandExecutionContext_Ref const&);
    void open_window();

private:
    void remove_paths_that_dont_exist_anymore();

private:
    Cool::CircularBuffer<std::filesystem::path> _list{100};
    Cool::ImGuiWindow                           _dialog_window;
    std::string                                 _filter{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("List", _list)
        );
    }
};

} // namespace Lab