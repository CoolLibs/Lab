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
        .name      = "Saturation",
        .signature = {
            .from = PrimitiveType::Color{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
return mix(vec3(dot(in1, vec3(0.2126, 0.7152, 0.0722))), in1, `Saturation`);
    )STR"},
        .inputs        = {},
        .properties    = {Cool::InputDefinition<float>{"`Saturation`"}},
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
    `Image`(uv + vec2(-1. * `Drift`, 0.0)).r,
    `Image`(uv + vec2( 0. * `Drift`, 0.0)).g,
    `Image`(uv + vec2(+1. * `Drift`, 0.0)).b
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
        .properties = {Cool::InputDefinition<float>{"`Drift`"}},
    }});

    this->add_definition({{
        .name      = "Over",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Color{},
        },
        .function_body = {R"STR(
vec2 uv = in1;
return mix(`Under`(uv), `Over`(uv), `Over Opacity`);
    )STR"},
        .inputs        = {
            {{
                       .name      = "`Over`",
                       .signature = {
                           .from = PrimitiveType::UV{},
                           .to   = PrimitiveType::Color{},
                },
            }},
            {{
                       .name      = "`Under`",
                       .signature = {
                           .from = PrimitiveType::UV{},
                           .to   = PrimitiveType::Color{},
                },
            }},
        },
        .properties = {Cool::InputDefinition<float>{"`Over Opacity`"}},
    }});

    this->add_definition({{
        .name      = "Time",
        .signature = {
            .from = PrimitiveType::Void{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return _time;
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});

    this->add_definition({{
        .name      = "Sine (Normalized)",
        .signature = {
            .from = PrimitiveType::Float{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return sin((in1 - 0.25) * 6.28) * 0.5 + 0.5;
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});

    this->add_definition({{
        .name      = "Multiply (float)",
        .signature = {
            .from = PrimitiveType::Void{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return `A` * `B`;
    )STR"},
        .inputs        = {},
        .properties    = {
            Cool::InputDefinition<float>{"`A`"},
            Cool::InputDefinition<float>{"`B`"},
        },
    }});

    this->add_definition({{
        .name      = "Vertical Gradient",
        .signature = {
            .from = PrimitiveType::UV{},
            .to   = PrimitiveType::Float{},
        },
        .function_body = {R"STR(
return in1.y*0.5+0.5;
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});
}

} // namespace Lab