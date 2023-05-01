#if COOLLAB_HAS_OPENSSL
#include "post_image_online.h"
#include <string>
#include "Cool/File/File.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Path/Path.h"
#include "Cool/String/String.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"

static auto escape(std::string str) -> std::string
{
    // First, escape the backslashes so that we don't later escape backslashes that are actually meant to escape something.
    Cool::String::replace_all(str, "\\", "\\\\");
    Cool::String::replace_all(str, "=", "\\=");
    Cool::String::replace_all(str, "|", "\\|");

    return str;
}

#if LAB_ENABLE_TESTS
#include <doctest/doctest.h>
TEST_CASE("escape()")
{
    CHECK(escape("Hello") == "Hello");
    CHECK(escape("=") == "\\=");
    CHECK(escape("|") == "\\|");
    CHECK(escape("\\") == "\\\\");
    CHECK(escape("\\=") == "\\\\\\=");
    CHECK(escape("Hello\\=Hello") == "Hello\\\\\\=Hello");
}
#endif

namespace Lab {

void post_image_online(ArtworkInfo const& artwork_info, AuthorInfo const& author_info, LegalInfo const& legal_info, std::string const& image_png_data)
{
    auto cli = httplib::SSLClient{"api.cloudinary.com"};

    // Create the multipart/form-data request
    auto const items = httplib::MultipartFormDataItems{
        // Add the image file as a binary data item
        httplib::MultipartFormData{
            .name         = "file",
            .content      = image_png_data,
            .filename     = "image.png",
            .content_type = "image/png",
        },
        httplib::MultipartFormData{
            .name         = "upload_preset",
            .content      = "gallery",
            .filename     = {},
            .content_type = {},
        },
        httplib::MultipartFormData{
            .name         = "tags",
            .content      = "gallery",
            .filename     = {},
            .content_type = {},
        },
        httplib::MultipartFormData{
            .name    = "context",
            .content = fmt::format(
                "title={}|description={}|author_name={}|author_link={}|email={}|agreed_to_have_it_shared_on_our_instagram={}",
                escape(artwork_info.title),
                escape(artwork_info.description),
                escape(author_info.name),
                escape(author_info.link),
                escape(legal_info.email),
                escape(legal_info.has_agreed_to_share_on_instagram ? "true" : "false")
            ),
            .filename     = {},
            .content_type = {},
        },
    };

    // Send the POST request with the multipart/form-data
    auto const res = cli.Post("/v1_1/coollab/image/upload", items);
    if (res && res->status == 200)
    {
        Cool::Log::ToUser::info(
            "Gallery",
            "Posted successfully.\nYou can now see your image online at https://coollab-art.com/Gallery.",
            std::vector{
                Cool::ClipboardContent{
                    .title   = "link",
                    .content = "https://coollab-art.com/Gallery",
                },
            }
        );
    }
    else
    {
        if (!res)
            Cool::Log::ToUser::warning("Gallery", fmt::format("Failed to post.\n{}", httplib::to_string(res.error())));
        else
            Cool::Log::ToUser::warning("Gallery", fmt::format("Failed to post.\nStatus {}.\n{}: {}", res->status, res->reason, res->body));
    }
}

} // namespace Lab

#else
namespace Lab {
void post_image_online(ArtworkInfo const&, AuthorInfo const&, std::string const&)
{
    assert(false && "CoolLab was not built with the OpenSSL library because it was not found while compiling. You cannot use this function.");
}
} // namespace Lab
#endif