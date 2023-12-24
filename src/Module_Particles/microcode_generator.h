#include <string>

namespace Lab {

auto buffer_to_vec(int dimension, std::string const& buffer_name, std::string const& id_name) -> std::string;
auto vec_to_buffer(int dimension, std::string const& buffer_name, std::string const& id_name, std::string const& vec_name) -> std::string;
auto vec_zero(int dimension) -> std::string;

}; // namespace Lab
