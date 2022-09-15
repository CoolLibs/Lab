#if LAB_ENABLE_TESTS
#include <doctest/doctest.h>
#include "NodeTemplate.h"

using namespace Lab;

// float main(RgbColor color)
// {
//     return dot(color, vec3(0.2126, 0.7152, 0.0722));
// }
static const auto black_and_white = NodeTemplate{
    .name      = "Black & White",
    .signature = {
        .from = {PrimitiveType::RgbColor},
        .to   = PrimitiveType::Float,
    },
    .function_body = {R"STR(
return dot(color, vec3(0.2126, 0.7152, 0.0722));
    )STR"},
};

#endif