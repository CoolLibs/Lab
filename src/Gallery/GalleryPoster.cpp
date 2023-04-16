#include "GalleryPoster.h"
#include <imgui.h>
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Image/compute_image_size.h"
#include "Gallery/GalleryPoster.h"
#include "Gallery/post_image_online.h"
#include "glm/gtc/type_ptr.hpp"

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

void GalleryPoster::imgui_open_sharing_form(std::optional<Cool::AspectRatio> const& aspect_ratio)
{
    Cool::ImGuiExtras::maybe_disabled(
        !has_openssl,
        "DEV ONLY: We didn't find the OpenSSL library on your machine while compiling CoolLab so this feature was disabled.\nLook at how to install OpenSSL on your computer if you want this feature.", [&]() {
            if (ImGui::Button(Cool::icon_fmt("Share online", ICOMOON_EARTH, true).c_str()))
            {
                _window.open();
                if (aspect_ratio)
                    _aspect_ratio = *aspect_ratio;
            }
        }
    );
}

void GalleryPoster::imgui_window(std::function<std::string(img::Size)> const& render_png)
{
    _window.show([&]() {
        ImGui::SeparatorText("Artwork");
        _artwork_info.imgui();
        _aspect_ratio.imgui(0.f, "Aspect Ratio");
        {
            auto const size = export_size();
            auto       sz   = glm::ivec2{
                static_cast<int>(size.width()),
                static_cast<int>(size.height()),
            };
            ImGui::BeginDisabled();
            ImGui::DragInt2("Size", glm::value_ptr(sz));
            ImGui::EndDisabled();
            ImGui::SameLine();
            Cool::ImGuiExtras::help_marker("You cannot choose the size of your image because we don't want to be storing too big images.\nYou can only control the Aspect Ratio above.");
        }
        ImGui::SeparatorText("Author");
        _author_info.imgui();
        ImGui::SeparatorText("");
        if (ImGui::Button(Cool::icon_fmt("Send", ICOMOON_EARTH).c_str()))
        {
            post_image_online(_artwork_info, _author_info, render_png(export_size()));
            _window.close();
            _artwork_info = {}; // Clear info that is related to one specific artwork
        }
    });
}

auto GalleryPoster::export_size() const -> img::Size
{
    return Cool::compute_image_size(_aspect_ratio.get(), 2'250'000.f);
}

} // namespace Lab