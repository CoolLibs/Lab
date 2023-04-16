#include "GalleryPoster.h"
#include <imgui.h>
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Gallery/GalleryPoster.h"
#include "Gallery/post_image_online.h"

namespace Lab {

GalleryPoster::GalleryPoster()
    : _window{Cool::icon_fmt("Share online", ICOMOON_EARTH), false}
{
}

static constexpr bool has_openssl =
#if COOLLAB_HAS_OPENSSL
    true;
#else
    false;
#endif

void GalleryPoster::imgui_open_sharing_form()
{
    Cool::ImGuiExtras::maybe_disabled(
        !has_openssl,
        "DEV ONLY: We didn't find the OpenSSL library on your machine while compiling CoolLab so this feature was disabled.\nLook at how to install OpenSSL on your computer if you want this feature.", [&]() {
            if (ImGui::Button(Cool::icon_fmt("Share online", ICOMOON_EARTH, true).c_str()))
            {
                _window.open();
            }
        }
    );
}

void GalleryPoster::imgui_window(std::function<std::string(img::Size)> const& render_png)
{
    _window.show([&]() {
        ImGui::SeparatorText("Artwork");
        _artwork_info.imgui();
        ImGui::SeparatorText("Author");
        _author_info.imgui();
        ImGui::SeparatorText("");
        if (ImGui::Button(Cool::icon_fmt("Send", ICOMOON_EARTH).c_str()))
        {
            post_image_online(_artwork_info, _author_info, render_png(img::Size{1000, 1000}));
            _window.close();
            _artwork_info = {}; // Clear info that is related to one specific artwork
        }
    });
}

} // namespace Lab