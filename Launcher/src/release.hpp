#pragma once
#include <nlohmann/json.hpp>
#include <tl/expected.hpp>

using json = nlohmann::json;

auto get_release(std::string_view const& version) -> tl::expected<nlohmann::basic_json<>, std::string>;
auto get_coollab_download_url(nlohmann::basic_json<> const& release) -> std::string;
