#pragma once

namespace Lab {

auto buffer_to_vec(int dimension, std::string_view buffer_name, std::string_view idx_name) -> std::string;
auto vec_to_buffer(int dimension, std::string_view buffer_name, std::string_view idx_name, std::string_view vec_name) -> std::string;

}; // namespace Lab
