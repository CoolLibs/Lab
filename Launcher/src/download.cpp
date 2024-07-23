#include <curl/curl.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <vector>
#include "download.hpp"
#include "release.hpp"
#include "utils.hpp"

namespace fs = std::__fs::filesystem;

// download file from url
auto download_zip(nlohmann::basic_json<> const& release) -> tl::expected<std::vector<char>, std::string>
{
    auto const url = get_coollab_download_url(release);
    std::cout << url << std::endl;
    CURL* curl;
    curl = curl_easy_init();
    std::vector<char> out_data;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out_data);
        CURLcode res;
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK)
            return out_data;
        else
        {
            std::cerr << "Failed to download zip file from URL: " << url << std::endl;
            std::cerr << curl_easy_strerror(res) << std::endl;
            return tl::make_unexpected("error"); // TODO : set a correct error message
        }
    }
    else
    {
        std::cerr << "Failed to initialize curl." << std::endl;
        return tl::make_unexpected("error"); // TODO : set a correct error message
    }
}

auto install_macos_dependencies_if_necessary() -> void
{
    // Check if Homebrew is already installed
    if (std::system("command -v brew >/dev/null 2>&1"))
        install_homebrew();
    else
        std::cout << "Homebrew is already installed." << std::endl;

    // Check if FFmpeg is already installed
    if (std::system("command -v ffmpeg >/dev/null 2>&1"))
        install_ffmpeg();
    else
        std::cout << "FFmpeg is already installed." << std::endl;
}

auto install_homebrew() -> void
{
    std::cout << "Installing Homebrew..." << std::endl;
    int result = std::system(
        "/bin/bash -c \"$(curl -fsSL "
        "https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\""
    );
    if (result != 0)
    {
        std::cerr << "Homebrew installation failed!" << std::endl;
        exit(result);
    }
    std::cout << "Homebrew successfully installed." << std::endl;
}

auto install_ffmpeg() -> void
{
    std::cout << "Installing FFmpeg via Homebrew..." << std::endl;
    int result = std::system("brew install ffmpeg");
    if (result != 0)
    {
        std::cerr << "FFmpeg installation failed!" << std::endl;
        exit(result);
    }
    std::cout << "FFmpeg successfully installed." << std::endl;
}

auto coollab_version_is_installed(std::string_view const& version) -> bool
{
    return fs::exists(get_PATH() + std::string(version));
}