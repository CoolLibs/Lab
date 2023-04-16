#pragma once
#include "ArtworkInfo.h"
#include "AuthorInfo.h"
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/Image/AspectRatio.h"

namespace Lab {

class GalleryPoster {
public:
    GalleryPoster();

    void imgui_open_sharing_form(std::optional<Cool::AspectRatio> const&);
    /// `render` is a function that renders an image to a .png and returns it in a string.
    void imgui_window(std::function<std::string(img::Size)> const& render_png);

private:
    [[nodiscard]] auto export_size() const -> img::Size;

private:
    Cool::ImGuiWindow _window;
    ArtworkInfo       _artwork_info{};
    AuthorInfo        _author_info{};
    Cool::AspectRatio _aspect_ratio{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Artwork info", _artwork_info),
            cereal::make_nvp("Author info", _author_info),
            cereal::make_nvp("Aspect Ratio", _aspect_ratio)
        );
    }
};

} // namespace Lab