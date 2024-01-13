#include "simulation_code_gen_helpers.h"
#include <sstream>

namespace Lab {

auto buffer_to_vec(int dimension, std::string_view buffer_name, std::string_view idx_name) -> std::string
{
    using fmt::literals::operator""_a;
    assert(2 <= dimension && dimension <= 4);

    auto res = std::stringstream{};
    res << "vec"
        << dimension
        << '(';
    for (int i = 0; i < dimension; i++)
    {
        res << fmt::format(
            FMT_COMPILE("{buffer_name}[{idx_name} * {dimension} + {i}]"),
            "buffer_name"_a = buffer_name,
            "idx_name"_a    = idx_name,
            "dimension"_a   = dimension,
            "i"_a           = i
        );
        if (i < dimension - 1)
            res << ", ";
    }
    res << ')';
    return res.str();
}

auto vec_to_buffer(int dimension, std::string_view buffer_name, std::string_view idx_name, std::string_view vec_name) -> std::string
{
    using fmt::literals::operator""_a;
    assert(2 <= dimension && dimension <= 4);

    auto res = std::stringstream{};
    for (int i = 0; i < dimension; i++)
    {
        res << fmt::format(
            FMT_COMPILE("{buffer_name}[{idx_name} * {dimension} + {i}] = {vec_name}[{i}];\n"),
            "buffer_name"_a = buffer_name,
            "idx_name"_a    = idx_name,
            "dimension"_a   = dimension,
            "i"_a           = i,
            "vec_name"_a    = vec_name
        );
    }
    return res.str();
}

}; // namespace Lab