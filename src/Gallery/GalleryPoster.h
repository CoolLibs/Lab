#pragma once
#include "ArtworkInfo.h"
#include "Cool/ImGui/ImGuiWindow.h"

namespace Lab {

class GalleryPoster {
public:
    GalleryPoster();

    void imgui_open_sharing_form();
    void imgui_window();

private:
    Cool::ImGuiWindow _window;
    ArtworkInfo       _artwork_info{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Artwork info", _artwork_info)
        );
    }
};

} // namespace Lab