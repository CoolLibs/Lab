#include "release.hpp"
#include <curl/curl.h>
#include <iostream>
#include <string_view>
#include <tl/expected.hpp>
#include "fmt/core.h"
#include "utils.hpp"

// get assets from the latest coollab release
// auto get_last_release_assets(const std::string& repo) -> json
// {
//     CURL*       curl;
//     CURLcode    res;
//     std::string readBuffer;
//     curl = curl_easy_init();
//     if (curl)
//     {
//         std::string url = "https://api.github.com/repos/" + repo + "/releases/tags/launcher-test-4";
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//         curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.68.0");
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
//         res = curl_easy_perform(curl);
//         curl_easy_cleanup(curl);

//         if (res != CURLE_OK)
//         {
//             std::cerr << "Failed to fetch latest release info: "
//                       << curl_easy_strerror(res) << std::endl;
//             return "";
//         }
//     }

//     // Parse JSON response
//     try
//     {
//         auto jsonResponse = json::parse(readBuffer);
//         if (!jsonResponse.contains("assets") || jsonResponse["assets"].empty())
//         {
//             std::cerr << "No assets found in the latest release." << std::endl;
//             return "";
//         }
//         auto assets = jsonResponse["assets"];
//         // std::cout << assets.dump(4) << std::endl;
//         return assets;
//     }
//     catch (json::parse_error& e)
//     {
//         std::cerr << "JSON parse error: " << e.what() << std::endl;
//         return "";
//     }
//     catch (std::exception& e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return "";
//     }
// }

auto get_release(std::string_view const& version) -> tl::expected<nlohmann::basic_json<>, std::string>
{
    CURL* curl = curl_easy_init();
    if (!curl)
        return "";
    // return tl::make_unexpected(fmt::format("Failed to init curl: {}", curl_easy_strerror(res))); // TODO(Launcher) Better error message (query curl to know why it failed ?)

    std::string const url = fmt::format("https://api.github.com/repos/CoolLibs/Lab/releases/tags/{}", version); // TODO(Launcher) Don't hardcode the url to the release
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.68.0");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, string_write_callback);
    std::string readBuffer;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return tl::make_unexpected(""); // TODO : mettre un bon retour d'erreur
    // return tl::make_unexpected(fmt::format("Failed to fetch latest release info: {}", curl_easy_strerror(res)));

    // Parse JSON response
    try
    {
        auto const jsonResponse = json::parse(readBuffer);
        if (!jsonResponse.contains("assets") || jsonResponse["assets"].empty())
        {
            std::cerr << "No assets found in the latest release." << std::endl; // TODO(Launcher) use expected
            return tl::make_unexpected("");                                     // TODO : mettre un bon retour d'erreur
        }
        nlohmann::basic_json<> const& assets = jsonResponse["assets"];
        return assets;
    }
    catch (json::parse_error const& e)
    {
        std::cerr << "JSON parse error: " << e.what() << std::endl; // TODO(Launcher) use expected
        return tl::make_unexpected("");                             // TODO : mettre un bon retour d'erreur
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl; // TODO(Launcher) use expected
        return tl::make_unexpected("");                  // TODO : mettre un bon retour d'erreur
    }
}

// get the correct url corresponding to the latest release assets
// auto get_last_release_url(const json& assets) -> std::string
// {
//     for (const auto& asset : assets)
//     {
//         std::cout << asset << std::endl;
//         // std::string name = asset["name"];
//         // if (name == os_path)
//         // {
//         //     return asset["browser_download_url"];
//         // }
//     }

//     // nothing?
//     return "";
// }

auto get_coollab_download_url(nlohmann::basic_json<> const& release) -> std::string
{
    auto os_path = get_OS();
    for (const auto& asset : release)
    {
        std::string url = asset["browser_download_url"];
        if (url.find(os_path) != std::string::npos)
            return url;
    }
}

auto get_last_release_name(const json& assets) -> std::string
{
    auto os_path = get_OS();

    for (const auto& asset : assets)
    {
        std::string name = asset["name"];
        if (name == os_path)
        {
            // Retirer l'extension .zip
            size_t last_dot = name.find_last_of(".");
            if (last_dot != std::string::npos && name.substr(last_dot) == ".zip")
            {
                name = name.substr(0, last_dot);
            }
            return name;
        }
    }

    // nothing?
    return "";
}
