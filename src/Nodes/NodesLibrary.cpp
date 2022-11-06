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
return dot(in1, vec3(0.2126, 0.7152, 0.0722));
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
return (int(in1.x*10.) + int(in1.y*10.)) % 2 == 0 ? 0. : 1.;
    )STR"},
        .inputs        = {},
    });

    this->add_definition({
        ._name     = "TestImage",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
return fract(vec3(in1, 0.));
    )STR"},
        .inputs        = {},
    });

    this->add_definition({
        ._name     = "Zoom Out",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::UV{},
        },
        .function_body = {R"STR(
return in1 * 10.;
    )STR"},
        .inputs        = {},
    });

    this->add_definition({
        ._name     = "Gradient Colorizer",
        .signature = {
            .from = PrimitiveType::Float{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
return mix(vec3(1., 0., 0.), vec3(0., 0., 1.), in1);
    )STR"},
        .inputs        = {},
        .properties    = {Cool::Input<Cool::Gradient>{"gradient"}},
    });
}

} // namespace Lab