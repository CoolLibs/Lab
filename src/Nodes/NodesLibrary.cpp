#include "NodesLibrary.h"

namespace Lab {

NodesLibrary::NodesLibrary()
{
    this->add_definition({
        ._name     = "Black & White",
        .signature = {
            .from = PrimitiveType::Color{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return dot(color, vec3(0.2126, 0.7152, 0.0722));
    )STR"},
        .inputs        = {},
    });

    this->add_definition({
        ._name     = "Checkerboard",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return int(in1.x + in1.y) % 2 == 0 ? 0. : 1.;
    )STR"},
        .inputs        = {},
    });
}

} // namespace Lab