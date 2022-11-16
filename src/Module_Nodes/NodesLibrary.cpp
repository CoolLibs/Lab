#include "NodesLibrary.h"

namespace Lab {

NodesLibrary::NodesLibrary()
{
    // TODO(JF) Once we start loading from file, catch exceptions thrown by definition creation, log the errors, and just don't add the given definition
    this->add_definition({{
        .name      = "Black & White",
        .signature = {
            .from = PrimitiveType::Color{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return dot(in1, vec3(0.2126, 0.7152, 0.0722));
    )STR"},
        .inputs        = {},
    }});

    this->add_definition({{
        .name      = "Checkerboard",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return (int(in1.x*10.) + int(in1.y*10.)) % 2 == 0 ? 0. : 1.;
    )STR"},
        .inputs        = {},
    }});

    this->add_definition({{
        .name      = "TestImage",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
return fract(vec3(in1, 0.));
    )STR"},
        .inputs        = {},
    }});

    this->add_definition({{
        .name      = "Zoom",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::UV{},
        },
        .function_body = {R"STR(
return in1 / `Zoom`;
    )STR"},
        .inputs        = {},
        .properties    = {Cool::InputDefinition<float>{"`Zoom`"}},
    }});

    this->add_definition({{
        .name      = "Gradient Colorizer",
        .signature = {
            .from = PrimitiveType::Float{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
return `Gradient`(in1).rgb;
    )STR"},
        .inputs        = {},
        .properties    = {Cool::InputDefinition<Cool::Gradient>{"`Gradient`"}},
    }});

    this->add_definition({{
        .name      = "Circle",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return smoothstep(`Edge Blur`, 0., length(in1) - `Radius`);
    )STR"},
        .inputs        = {},
        .properties    = {
            Cool::InputDefinition<float>{"`Radius`"},
            Cool::InputDefinition<float>{"`Edge Blur`"},
        },
    }});

    this->add_definition({{
        .name      = "Displacement",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::UV{},
        },
        .function_body = {R"STR(
in1.y += `Amplitude` * sin(in1.x * `Frequency`);
return in1;
    )STR"},
        .inputs        = {},
        .properties    = {
            Cool::InputDefinition<float>{"`Amplitude`"},
            Cool::InputDefinition<float>{"`Frequency`"},
        },
    }});

    this->add_definition({{
        .name      = "RGB Drift",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
vec2 uv = in1;
return vec3(
    `Image`(uv + vec2(0.0, 0.0)).r,
    `Image`(uv + vec2(0.1, 0.0)).g,
    `Image`(uv + vec2(0.2, 0.0)).b
);
    )STR"},
        .inputs        = {
            {{
                       .name      = "`Image`",
                       .signature = {
                           .from = PrimitiveType::UV{},
                           .to   = PrimitiveType::Color{},
                },
            }},
        },
        .properties = {},
    }});
}

} // namespace Lab