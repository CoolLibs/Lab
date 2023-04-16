#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Path/Path.h"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"
namespace Lab {

void post_image_online()
{
    httplib::SSLClient cli("api.cloudinary.com");

    // Read the image file contents
    std::string str = *Cool::File::to_string(Cool::Path::root() / "res/logo.png", std::ios::binary);

    // Create the multipart/form-data request
    auto items = httplib::MultipartFormDataItems{
        // Add the image file as a binary data item
        httplib::MultipartFormData{
            .name         = "file",
            .content      = str,
            .filename     = "image.png",
            .content_type = "image/png",
        },
        httplib::MultipartFormData{
            .name    = "upload_preset",
            .content = "gallery",
        },
        httplib::MultipartFormData{
            .name    = "tags",
            .content = "gallery",
        },
        httplib::MultipartFormData{
            .name    = "context",
            .content = "author_name=Jules Fouchy", // TODO(JF) escape | and = and \ characters with a \.
        },
    };

    // Send the POST request with the multipart/form-data
    auto const res = cli.Post("/v1_1/coollab/image/upload", items);
    // std::cout << res->body;
    if (res && res->status == 200)
    {
        Cool::Log::ToUser::info("Gallery", fmt::format("Posted successfully.\n{}", res->body));
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