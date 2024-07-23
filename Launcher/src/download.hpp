#pragma once
#include <string>
#include <tl/expected.hpp>
#include "nlohmann/json.hpp"

auto install_macos_dependencies_if_necessary() -> void;

// install essential packages
auto install_homebrew() -> void;
auto install_ffmpeg() -> void;

// check if Coollab version is already installed
auto coollab_version_is_installed(std::string_view const& version) -> bool;
// download zip and return it
auto download_zip(nlohmann::basic_json<> const& release) -> tl::expected<std::vector<char>, std::string>;