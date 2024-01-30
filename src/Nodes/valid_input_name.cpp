#include "valid_input_name.h"
#include "valid_glsl.h"

namespace Lab {

auto valid_input_name(Cool::AnyInput const& input) -> std::string
{
    using fmt::literals::operator""_a;
    return std::visit([](auto&& input) {
        return fmt::format(
            FMT_COMPILE(
                "{name}{id}"
            ),
            "name"_a = valid_glsl(input.name()),
            "id"_a   = valid_glsl(std::to_string(input.id())) // We use a unique id per input to make sure they don't clash with anything. For example if the node was called Zoom and its input was also called Zoom, both the function and the uniform variable would get the same name.
        );
    },
                      input);
}

} // namespace Lab