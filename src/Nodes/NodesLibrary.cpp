#include "NodesLibrary.h"

namespace Lab {

NodesLibrary::NodesLibrary()
{
    this->add_definition({
        ._name     = "Black & White",
        .signature = {
            .from = PrimitiveType::RgbColor{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return dot(color, vec3(0.2126, 0.7152, 0.0722));
    )STR"},
        .inputs        = {},
    });

    this->add_definition({
        ._name     = "Blop",
        .signature = {
            .from = PrimitiveType::Float{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return 1.;
    )STR"},
        .inputs        = {},
    });
}

} // namespace Lab