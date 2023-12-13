#include <string>

namespace Lab {

auto buffer_to_vec(size_t dimension, std::string const& buffer_name, std::string const& id_name) -> std::string;
auto vec_to_buffer(size_t dimension, std::string const& buffer_name, std::string const& id_name, std::string const& vec_name) -> std::string;
auto vec_zero(size_t dimension) -> std::string;

}; // namespace Lab
