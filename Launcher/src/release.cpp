#include <curl/curl.h>
#include <string_view>
#include <tl/expected.hpp>

#include "release.hpp"
#include "fmt/core.h"
#include "utils.hpp"

auto get_release(std::string_view const& version) -> tl::expected<nlohmann::basic_json<>, std::string>
{
    CURL* curl = curl_easy_init();
    if (!curl)
        return tl::make_unexpected("Failed to initialize curl");

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
        return tl::make_unexpected(fmt::format("Failed to fetch release info: {}", curl_easy_strerror(res)));

    // Parse JSON response
    try
    {
        auto const jsonResponse = json::parse(readBuffer);
        if (!jsonResponse.contains("assets") || jsonResponse["assets"].empty())
            return tl::make_unexpected("No assets found in the release.");
        nlohmann::basic_json<> const& assets = jsonResponse["assets"];
        return assets;
    }
    catch (json::parse_error const& e)
    {
        return tl::make_unexpected(fmt::format("JSON parse error: {}", e.what()));
    }
    catch (std::exception& e)
    {
        return tl::make_unexpected(fmt::format("Error: {}", e.what()));
    }
}
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