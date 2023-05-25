#include "GalleryPoster.h"
#include <imgui.h>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/ImGui/markdown.h"
#include "Cool/Image/compute_image_size.h"
#include "Gallery/GalleryPoster.h"
#include "Gallery/post_image_online.h"
#include "glm/gtc/type_ptr.hpp"

namespace Lab {

GalleryPoster::GalleryPoster()
    : _window{Cool::icon_fmt("Share online", ICOMOON_EARTH), Cool::ImGuiWindowConfig{.is_modal = true}}
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
    Cool::ImGuiExtras::disabled_if(
        !has_openssl,
        "DEV ONLY: We didn't find the OpenSSL library on your machine while compiling Coollab so this feature was disabled.\nLook at how to install OpenSSL on your computer if you want this feature.", [&]() {
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
        Cool::ImGuiExtras::markdown("Your image will be visible on [Coollab's gallery](https://coollab-art.com/Gallery).");
        Cool::ImGuiExtras::markdown("If you want to edit or remove it, send an email at [coollab.lib@gmail.com](mailto:coollab.lib@gmail.com) from the email address that you will provide below.");
        Cool::ImGuiExtras::separator_text("Artwork");
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
            Cool::ImGuiExtras::help_marker("You cannot choose the size of your image because we don't want to be storing too big images.\nYou can only control the Aspect Ratio above.");
        }
        Cool::ImGuiExtras::separator_text("Author");
        _author_info.imgui();
        Cool::ImGuiExtras::separator_text("Legal");
        _legal_info.imgui();
        Cool::ImGuiExtras::separator_text("");
        auto const missing_info_message = std::optional<std::string>{_legal_info.missing_information_message()};
        Cool::ImGuiExtras::disabled_if(missing_info_message.has_value(), missing_info_message.value_or("").c_str(), [&]() {
            if (ImGui::Button(Cool::icon_fmt("Send", ICOMOON_EARTH).c_str()))
            {
                post_image_online(_artwork_info, _author_info, _legal_info, render_png(export_size()));
                _window.close();
                _artwork_info = {}; // Clear info that is related to one specific artwork
                if (Cool::DebugOptions::public_exhibition_mode())
                {
                    _author_info = {}; // Clear info that is related to the author,
                    _legal_info  = {}; // because the next post will be made by another visitor.
                }
            }
        });
    });
}

auto GalleryPoster::export_size() const -> img::Size
{
    return Cool::compute_image_size(_aspect_ratio.get(), 2'250'000.f);
}

} // namespace Lab