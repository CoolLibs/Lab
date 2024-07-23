#pragma once
#include <string>
#include <tl/expected.hpp>
#include "nlohmann/json.hpp"

// download
// auto download_zip_to_memory(const std::string& url, std::vector<char>& out_data) -> bool;


auto install_macos_dependencies_if_necessary() -> void;
// install essential packages
auto install_homebrew() -> void;
auto install_ffmpeg() -> void;

// check if Coollab version is already installed
auto version_is_installed(const std::string& path, const std::string& version_name) -> bool;

//download zip and return it
auto download_zip(nlohmann::basic_json<> const &release) -> tl::expected<std::vector<char>, std::string>;