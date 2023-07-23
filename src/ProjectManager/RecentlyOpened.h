#pragma once
#include <Cool/Containers/CircularBuffer.h>
#include <filesystem>
#include <optional>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "Cool/ImGui/ImGuiWindow.h"

namespace Lab {

using PathsList = Cool::CircularBuffer<std::filesystem::path>;

class RecentlyOpened {
public:
    RecentlyOpened();

    void on_project_opened(std::filesystem::path);
    auto get_list() -> PathsList const&;
    void imgui_window(CommandExecutionContext_Ref const&);
    void open_window();

private:
    void remove_paths_that_dont_exist_anymore();

private:
    PathsList         _list{100};
    Cool::ImGuiWindow _dialog_window;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("List", _list)
        );
    }
};

} // namespace Lab