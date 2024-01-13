#include "simulation_code_gen_helpers.h"

namespace Lab {

auto buffer_to_vec(int dimension, std::string_view buffer_name, std::string_view idx_name) -> std::string
{
    assert(2 <= dimension && dimension <= 4);
    using fmt::literals::operator""_a;
    std::string content = "";
    for (int i = 0; i < dimension; i++)
    {
        content += fmt::format(
            FMT_COMPILE("{buffer_name}[{idx_name} * {dimension} + {i}]"),
            "buffer_name"_a = buffer_name,
            "idx_name"_a    = idx_name,
            "dimension"_a   = dimension,
            "i"_a           = i
        );
        if (i < dimension - 1)
        {
            content += ", ";
        }
    }
    std::string vec = fmt::format(
        FMT_COMPILE("vec{dimension}({content})"),
        "dimension"_a = dimension,
        "content"_a   = content
    );
    return vec;
}

auto vec_to_buffer(int dimension, std::string_view buffer_name, std::string_view idx_name, std::string_view vec_name) -> std::string
{
    assert(2 <= dimension && dimension <= 4);
    static constexpr std::array<const char*, 4> dimension_lookup = {"x", "y", "z", "w"};
    using fmt::literals::operator""_a;
    std::string output = "";
    for (size_t i = 0; i < static_cast<size_t>(dimension); i++)
    {
        output += fmt::format(
            FMT_COMPILE("{buffer_name}[{idx_name} * {dimension} + {buffer_i}] = {vec_name}.{vec_i};\n"),
            "buffer_name"_a = buffer_name,
            "idx_name"_a    = idx_name,
            "dimension"_a   = dimension,
            "buffer_i"_a    = i,
            "vec_name"_a    = vec_name,
            "vec_i"_a       = dimension_lookup.at(i)
        );
    }
    return output;
}

}; // namespace Lab