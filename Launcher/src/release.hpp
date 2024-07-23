#pragma once
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

using json = nlohmann::json;

// get data from github repo assets
auto get_last_release_assets(const std::string& repo) -> json;
auto get_last_release_url(const json& assets) -> std::string;
auto get_last_release_name(const json& assets) -> std::string;

// get release
auto get_release(std::string_view const& version) -> tl::expected<nlohmann::basic_json<>, std::string>;
auto get_coollab_download_url(nlohmann::basic_json<> const& release) -> std::string;
