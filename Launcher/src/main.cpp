#include <cstdlib>
#include <iostream>
#include <string_view>
#include "download.hpp"
#include "extractor.hpp"
#include "release.hpp"

auto main() -> int
{
#ifdef __APPLE__
    // Install Homebrew & FFmpeg on MacOS
    install_macos_dependencies_if_necessary();
#endif

    std::string_view requested_version = "launcher-test-4";
    // Install last Coollab release
    if (!coollab_version_is_installed(requested_version))
    {
        auto const release = get_release(requested_version);
        auto const zip     = download_zip(*release);
        try
        {
            extract_zip(*zip, requested_version);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception occurred: " << e.what() << std::endl;
        }
        std::cout << "✅ Coollab " << requested_version << " is installed! ";
    }
    else
        std::cout << "❌ Coollab " << requested_version << " is already installed." << std::endl;
}