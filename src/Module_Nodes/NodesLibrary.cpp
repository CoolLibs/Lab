#include "NodesLibrary.h"

namespace Lab {

NodesLibrary::NodesLibrary()
{
    // TODO(JF) Once we start loading from file, catch exceptions thrown by definition creation, log the errors, and just don't add the given definition
    this->add_definition({{
        .name      = "Black & White",
        .signature = {
            .from = PrimitiveType::Color,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
return dot(in1, vec3(0.2126, 0.7152, 0.0722));
    )STR"},
        .inputs        = {},
    }});

    this->add_definition({{
        .name      = "Saturation",
        .signature = {
            .from = PrimitiveType::Color,
            .to   = PrimitiveType::Color,
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
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
return (int(in1.x*10.) + int(in1.y*10.)) % 2 == 0 ? 0. : 1.;
    )STR"},
        .inputs        = {},
    }});

    this->add_definition({{
        .name      = "TestImage",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Color,
        },
        .function_body = {R"STR(
return fract(vec3(in1, 0.));
    )STR"},
        .inputs        = {},
    }});

    this->add_definition({{
        .name      = "Zoom",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::UV,
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
            .from = PrimitiveType::Float,
            .to   = PrimitiveType::Color,
        },
        .function_body = {R"STR(
return `Gradient`(in1).rgb;
    )STR"},
        .inputs        = {},
        .properties    = {Cool::InputDefinition<Cool::Gradient>{"`Gradient`"}},
    }});

    this->add_definition({{
        .name      = "Circle (Mask)",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
return smoothstep(`Edge Blur`, -`Edge Blur`, length(in1) - `Radius`);
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
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::UV,
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
        .name      = "Rotate 90°",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::UV,
        },
        .function_body = {R"STR(
return vec2(in1.y, -in1.x);
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});

    this->add_definition({{
        .name      = "Scale",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::UV,
        },
        .function_body = {R"STR(
return in1 * vec2(`Scale X`, `Scale Y`);
    )STR"},
        .inputs        = {},
        .properties    = {
            Cool::InputDefinition<float>{"`Scale X`"},
            Cool::InputDefinition<float>{"`Scale Y`"},
        },
    }});

    this->add_definition({{
        .name      = "RGB Drift",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Color,
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
                           .from = PrimitiveType::UV,
                           .to   = PrimitiveType::Color,
                },
            }},
        },
        .properties = {Cool::InputDefinition<float>{"`Drift`"}},
    }});

    this->add_definition({{
        .name      = "Over",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Color,
        },
        .function_body = {R"STR(
vec2 uv = in1;
return mix(`Under`(uv), `Over`(uv), `Over Opacity`);
    )STR"},
        .inputs        = {
            {{
                       .name      = "`Over`",
                       .signature = {
                           .from = PrimitiveType::UV,
                           .to   = PrimitiveType::Color,
                },
            }},
            {{
                       .name      = "`Under`",
                       .signature = {
                           .from = PrimitiveType::UV,
                           .to   = PrimitiveType::Color,
                },
            }},
        },
        .properties = {Cool::InputDefinition<float>{"`Over Opacity`"}},
    }});

    this->add_definition({{
        .name      = "Time",
        .signature = {
            .from = PrimitiveType::Void,
            .to   = PrimitiveType::Float,
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
            .from = PrimitiveType::Float,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
return sin((in1 - 0.25) * 6.28) * 0.5 + 0.5;
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});

    this->add_definition({{
        .name      = "Flip (x: 1 - x)",
        .signature = {
            .from = PrimitiveType::Float,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
return 1. - in1;
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});

    this->add_definition({{
        .name      = "Multiply (float)",
        .signature = {
            .from = PrimitiveType::Void,
            .to   = PrimitiveType::Float,
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
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
return in1.y*0.5+0.5;
    )STR"},
        .inputs        = {},
        .properties    = {},
    }});

    this->add_definition({{
        .name      = "Grid",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::UV,
        },
        .function_body  = {R"STR(
vec2 uv = unnormalize_uv_with_aspect_ratio(in1, 1.) * `Nb cells`;
vec2 gid = floor(uv);
`Index X` = gid.x;
`Index Y` = gid.y;
return normalize_uv_with_aspect_ratio(fract(uv), 1.);
    )STR"},
        .inputs         = {},
        .properties     = {Cool::InputDefinition<float>{"`Nb cells`"}},
        .output_indices = {
            "`Index X`",
            "`Index Y`",
        },
    }});

    this->add_definition({{
        .name      = "Render N Times",
        .signature = {
            .from = PrimitiveType::UV,
            .to   = PrimitiveType::Color,
        },
        .function_body = {R"STR(
vec3 color = vec3(0.);
for (int i = 0; i < `N`; ++i)
{
    `Index` = i;
    color += `Image`(in1);
}
return color;
    )STR"},
        .inputs        = {
            {{
                       .name      = "`Image`",
                       .signature = {
                           .from = PrimitiveType::UV,
                           .to   = PrimitiveType::Color,
                },
            }},
        },
        .properties     = {Cool::InputDefinition<int>{"`N`"}},
        .output_indices = {
            "`Index`",
        },
    }});

    this->add_definition({{
        .name      = "Random 2D",
        .signature = {
            .from = PrimitiveType::Void,
            .to   = PrimitiveType::Float,
        },
        .function_body = {R"STR(
vec2 seeds = vec2(`Seed X`, `Seed Y`);
float rand = fract(sin(dot(seeds, vec2(12.9898, 78.233))) * 43758.5453);
return rand * (`Max` - `Min`) + `Min`;
    )STR"},
        .properties    = {
            Cool::InputDefinition<float>{"`Seed X`"},
            Cool::InputDefinition<float>{"`Seed Y`"},
            Cool::InputDefinition<float>{"`Min`"},
            Cool::InputDefinition<float>{"`Max`"},
        },
    }});
}

} // namespace Lab