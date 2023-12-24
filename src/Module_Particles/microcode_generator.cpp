#include "microcode_generator.h"
#include <fmt/compile.h>
#include <array>
#include <cassert>
#include <string>
#include "fmt/core.h"

namespace Lab {

auto buffer_to_vec(int dimension, std::string const& buffer_name, std::string const& id_name) -> std::string
{
    assert(2 <= dimension && dimension <= 4);
    using fmt::literals::operator""_a;
    std::string content = "";
    for (int i = 0; i < dimension; i++)
    {
        content += fmt::format(
            FMT_COMPILE("{buffer_name}[{id_name} * {dimension} + {i}]"),
            "buffer_name"_a = buffer_name,
            "id_name"_a     = id_name,
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

auto vec_to_buffer(int dimension, std::string const& buffer_name, std::string const& id_name, std::string const& vec_name) -> std::string
{
    assert(2 <= dimension && dimension <= 4);
    static constexpr std::array<const char*, 4> dimension_lookup = {"x", "y", "z", "w"};
    using fmt::literals::operator""_a;
    std::string output = "";
    for (int i = 0; i < dimension; i++)
    {
        output += fmt::format(
            FMT_COMPILE("{buffer_name}[{id_name} * {dimension} + {buffer_i}] = {vec_name}.{vec_i};\n"),
            "buffer_name"_a = buffer_name,
            "id_name"_a     = id_name,
            "dimension"_a   = dimension,
            "buffer_i"_a    = i,
            "vec_name"_a    = vec_name,
            "vec_i"_a       = dimension_lookup.at(i)
        );
    }
    return output;
}

auto vec_zero(int dimension) -> std::string
{
    assert(2 <= dimension && dimension <= 4);
    using fmt::literals::operator""_a;
    return fmt::format("vec{n}(0.)", "n"_a = dimension);
}

}; // namespace Lab
