#include <iostream>
#include <string_view>
#include "download.hpp"
#include "extractor.hpp"
#include "release.hpp"

auto main() -> int
{
    // Install Homebrew & FFmpeg on MacOS
#ifdef __APPLE__
    install_macos_dependencies_if_necessary();
#endif

    std::string_view requested_version = "launcher-test-4";
    // Install last Coollab release
    auto const release = get_release(requested_version); // -> assets
    auto const zip = download_zip(*release);
    // auto const zip = download_zip(release);
    // download_zip(release); // ->
    // auto const data    = extract_zip(zip); // ->

    // std::string repo = "CoolLibs/Lab";

    // json        coollab_version = get_last_release_assets(repo);
    // std::string latest_coollab_version_url =
    //     get_last_release_url(coollab_version);
    // std::string latest_coollab_version_name =
    //     get_last_release_name(coollab_version);

    // if (latest_coollab_version_url.empty())
    // {
    //     std::cerr << "Failed to get the latest release URL." << std::endl;
    //     return 1;
    // }

    // #ifdef __APPLE__
    //     std::string zip_extract_path = "/Applications/Coollab/";
    // #elif defined(_WIN32)
    //     std::string zip_extract_path = "C:\\Program Files\\Coolab\\";
    // #elif defined(__linux__)
    //     std::string zip_extract_path = "/usr/local/Coollab/";
    // #else
    //     std::cerr << "Unsupported OS" << std::endl;
    //     return 1;
    // #endif

    //     if (version_is_installed(zip_extract_path, latest_coollab_version_name))
    //         std::cout << "Coollab version " << latest_coollab_version_name
    //                   << " is already installed." << std::endl;
    //     else
    //     {
    //         if (!extract_zip(latest_coollab_version_url, zip_extract_path, latest_coollab_version_name))
    //             std::cerr << "❌ Extraction failed." << std::endl;
    //         else
    //             std::cout << "✅ Successful extraction at : "
    //                       << zip_extract_path + latest_coollab_version_name << std::endl;
    //         return 0;
    //     }
}