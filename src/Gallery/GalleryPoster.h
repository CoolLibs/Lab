#pragma once
#include "ArtworkInfo.h"
#include "AuthorInfo.h"
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/Image/SharedAspectRatio.hpp"
#include "LegalInfo.h"

namespace Lab {

class GalleryPoster {
public:
    GalleryPoster();

    void imgui_open_sharing_form();
    /// `render` is a function that renders an image to a .png and returns it in a string.
    void imgui_window(std::function<std::string(img::Size)> const& render_png);

    void set_shared_aspect_ratio(Cool::SharedAspectRatio& shared_aspect_ratio) { _shared_aspect_ratio = &shared_aspect_ratio; }

private:
    [[nodiscard]] auto export_size() const -> img::Size;

private:
    Cool::ImGuiWindow        _window;
    ArtworkInfo              _artwork_info{};
    AuthorInfo               _author_info{};
    LegalInfo                _legal_info{};
    Cool::SharedAspectRatio* _shared_aspect_ratio{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Artwork info", _artwork_info),
            ser20::make_nvp("Author info", _author_info),
            ser20::make_nvp("Legal info", _legal_info)
        );
    }
};

} // namespace Lab