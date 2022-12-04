#include "NodesLibrary.h"
#include <string>
#include "Cool/StrongTypes/RgbColor.h"
#include "Module_Nodes/FunctionSignature.h"
#include "Module_Nodes/PrimitiveType.h"
#include "parse_node_definition.h"

namespace Lab {

void NodesLibrary::add_node_definition(NodeDefinition_Data definition)
{
    // HACK to apply pre-divide / post-multiply to rgb post-process effects
    if (definition.main_function.signature.signature == Signature::RGBTransformation)
    {
        auto const base_name = fmt::format("RGB{}", definition.main_function.name);
        definition.helper_functions.push_back(FunctionPieces{
            .name      = base_name,
            .signature = make_complete_function_signature(definition.main_function.signature),
            .body      = definition.main_function.body,
        });

        definition.main_function.signature.signature = Signature::RGBATransformation;
        definition.main_function.body                = fmt::format(
            R"STR(
    if (in1.a < 0.000001)
        return in1;
    vec3 rgb = in1.rgb / in1.a;
    rgb      = {}(rgb);
    return vec4(rgb * in1.a, in1.a);
)STR",
            base_name
        );
    }

    Cool::NodesLibrary<NodeDefinition>::add_definition(definition);
}

NodesLibrary::NodesLibrary()
{
    this->add_node_definition(*parse_node_definition("Add Test", R"STR(
INPUT float my_input;

float main(float x)
{
    return add(x, 1.);
}

float add(float x, float y){
    return x + y;
}

)STR"));

    this->add_node_definition(*parse_node_definition("Time Test", R"STR(
float main()
{
    return _time;
}
)STR"));
    this->add_node_definition(*parse_node_definition("Time2 Test", R"STR(
float main( )
{
    return _time;
}
)STR"));
    this->add_node_definition(*parse_node_definition("Time3 Test", R"STR(
float main(   )
{
    return _time;
}
)STR"));
    // TODO(JF) Once we start loading from file, catch exceptions thrown by definition creation, log the errors, and just don't add the given definition
    //     this->add_node_definition({
    //         .name      = "Black & White",
    //         .signature = {
    //             .from  = PrimitiveType::RGB,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return dot(in1, vec3(0.2126, 0.7152, 0.0722));
    //     )STR"},
    //         .inputs        = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Saturation",
    //         .signature = {
    //             .from  = PrimitiveType::RGB,
    //             .to    = PrimitiveType::RGB,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return mix(vec3(dot(in1, vec3(0.2126, 0.7152, 0.0722))), in1, `Saturation`);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<float>{"`Saturation`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Gamma",
    //         .signature = {
    //             .from  = PrimitiveType::RGB,
    //             .to    = PrimitiveType::RGB,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return pow(in1, vec3(`Gamma`));
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<float>{"`Gamma`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Brightness",
    //         .signature = {
    //             .from  = PrimitiveType::RGB,
    //             .to    = PrimitiveType::RGB,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return in1 + `Brightness`;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<float>{"`Brightness`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Red Channel",
    //         .signature = {
    //             .from  = PrimitiveType::RGB,
    //             .to    = PrimitiveType::RGB,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return vec3(in1.r, 0., 0.);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Checkerboard",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return (int(in1.x*10.) + int(in1.y*10.)) % 2 == 0 ? 0. : 1.;
    //     )STR"},
    //         .inputs        = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Test Image",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::RGB,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return fract(vec3(in1, 0.));
    //     )STR"},
    //         .inputs        = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Color",
    //         .signature = {
    //             .from  = PrimitiveType::Void,
    //             .to    = PrimitiveType::RGBA,
    //             .arity = 0,
    //         },
    //         .function_body = {R"STR(
    // return `Color`;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<Cool::PremultipliedRgbaColor>{"`Color`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Zoom",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return in1 / `Zoom`;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<float>{"`Zoom`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Translation",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return in1 + `Translation`;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<Cool::Point2D>{"`Translation`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Gradient Colorizer",
    //         .signature = {
    //             .from  = PrimitiveType::Float,
    //             .to    = PrimitiveType::RGBA,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return `Gradient`(in1);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<Cool::Gradient>{"`Gradient`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Circle (Mask)",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return smoothstep(`Edge Blur`, -`Edge Blur`, length(in1 - `Center`) - `Radius`);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {
    //             Cool::InputDefinition<float>{"`Radius`"},
    //             Cool::InputDefinition<float>{"`Edge Blur`"},
    //             Cool::InputDefinition<Cool::Point2D>{"`Center`"},
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Circle (Curve)",
    //         .signature = {
    //             .from  = PrimitiveType::Float,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // float angle = 6.28 * in1;
    // return `Center` + `Radius` * vec2(cos(angle), sin(angle));
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {
    //             Cool::InputDefinition<float>{"`Radius`"},
    //             Cool::InputDefinition<Cool::Point2D>{"`Center`"},
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Displacement",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // in1.y += `Amplitude` * sin(in1.x * `Frequency`);
    // return in1;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {
    //             Cool::InputDefinition<float>{"`Amplitude`"},
    //             Cool::InputDefinition<float>{"`Frequency`"},
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Rotate 90°",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return vec2(in1.y, -in1.x);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Scale",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return in1 * vec2(`Scale X`, `Scale Y`);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {
    //             Cool::InputDefinition<float>{"`Scale X`"},
    //             Cool::InputDefinition<float>{"`Scale Y`"},
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "RGB Drift",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::RGBA,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // vec2 uv = in1;
    // vec4 R = `Image`(uv + vec2(-1. * `Drift`, 0.0));
    // vec4 G = `Image`(uv + vec2( 0. * `Drift`, 0.0));
    // vec4 B = `Image`(uv + vec2(+1. * `Drift`, 0.0));
    // return vec4(
    //     R.r,
    //     G.g,
    //     B.b,
    //     max(max(R.a, G.a), B.a)
    // );
    //     )STR"},
    //         .inputs        = {
    //             {{
    //                        .name      = "`Image`",
    //                        .signature = {
    //                            .from = PrimitiveType::UV,
    //                            .to   = PrimitiveType::RGBA,
    //                 },
    //             }},
    //         },
    //         .properties = {Cool::InputDefinition<float>{"`Drift`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Time",
    //         .signature = {
    //             .from  = PrimitiveType::Void,
    //             .to    = PrimitiveType::Float,
    //             .arity = 0,
    //         },
    //         .function_body = {R"STR(
    // return _time * `Speed` + `Offset`;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {
    //             Cool::InputDefinition<float>{"`Speed`"},
    //             Cool::InputDefinition<float>{"`Offset`"},
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Sine (Normalized)",
    //         .signature = {
    //             .from  = PrimitiveType::Float,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return sin((in1 - 0.25) * 6.28) * 0.5 + 0.5;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Pow",
    //         .signature = {
    //             .from  = PrimitiveType::Float,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return pow(in1, `Exponent`);
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {Cool::InputDefinition<float>{"`Exponent`"}},
    //     });

    //     this->add_node_definition({
    //         .name      = "Flip (x: 1 - x)",
    //         .signature = {
    //             .from  = PrimitiveType::Float,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return 1. - in1;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Constant Float",
    //         .signature = {
    //             .from  = PrimitiveType::Void,
    //             .to    = PrimitiveType::Float,
    //             .arity = 0,
    //         },
    //         .function_body  = {"return `x`;"},
    //         .inputs         = {},
    //         .properties     = {Cool::InputDefinition<float>{"`x`"}},
    //         .output_indices = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Constant RGB",
    //         .signature = {
    //             .from  = PrimitiveType::Void,
    //             .to    = PrimitiveType::RGB,
    //             .arity = 0,
    //         },
    //         .function_body  = {"return `Color`;"},
    //         .inputs         = {},
    //         .properties     = {Cool::InputDefinition<Cool::RgbColor>{"`Color`"}},
    //         .output_indices = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Multiply",
    //         .signature = {
    //             .from  = PrimitiveType::Any,
    //             .to    = PrimitiveType::Any,
    //             .arity = 2,
    //         },
    //         .function_body = {R"STR(
    // return in1 * in2;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Vertical Gradient",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::Float,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // return in1.y*0.5+0.5;
    //     )STR"},
    //         .inputs        = {},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Grid",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::UV,
    //             .arity = 1,
    //         },
    //         .function_body  = {R"STR(
    // vec2 uv = unnormalize_uv_with_aspect_ratio(in1, 1.) * `Nb cells`;
    // vec2 gid = floor(uv);
    // `Index X` = gid.x;
    // `Index Y` = gid.y;
    // return normalize_uv_with_aspect_ratio(fract(uv), 1.);
    //     )STR"},
    //         .inputs         = {},
    //         .properties     = {Cool::InputDefinition<float>{"`Nb cells`"}},
    //         .output_indices = {
    //             "`Index X`",
    //             "`Index Y`",
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Render N Times",
    //         .signature = {
    //             .from  = PrimitiveType::UV,
    //             .to    = PrimitiveType::RGBA,
    //             .arity = 1,
    //         },
    //         .function_body = {R"STR(
    // vec4 color = vec4(0.);
    // for (int i = 0; i < `N`; ++i)
    // {
    //     `Index` = i;
    //     color = `Blend Mode`(`Image`(in1), color);
    // }
    // return color;
    //     )STR"},
    //         .inputs        = {
    //             {{.name = "`Image`", .signature = Signature::ImageRGBA}},
    //             {{.name = "`Blend Mode`", .signature = Signature::BlendMode}},
    //         },
    //         .properties     = {Cool::InputDefinition<int>{"`N`"}},
    //         .output_indices = {
    //             "`Index`",
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Random 2D",
    //         .signature = {
    //             .from  = PrimitiveType::Void,
    //             .to    = PrimitiveType::Float,
    //             .arity = 0,
    //         },
    //         .function_body = {R"STR(
    // vec2 seeds = vec2(`Seed X`, `Seed Y`);
    // float rand = fract(sin(dot(seeds, vec2(12.9898, 78.233))) * 43758.5453);
    // return rand * (`Max` - `Min`) + `Min`;
    //     )STR"},
    //         .properties    = {
    //             Cool::InputDefinition<float>{"`Seed X`"},
    //             Cool::InputDefinition<float>{"`Seed Y`"},
    //             Cool::InputDefinition<float>{"`Min`"},
    //             Cool::InputDefinition<float>{"`Max`"},
    //         },
    //     });

    //     this->add_node_definition({
    //         .name      = "Over (Blend Mode)",
    //         .signature = {
    //             .from  = PrimitiveType::RGBA,
    //             .to    = PrimitiveType::RGBA,
    //             .arity = 2,
    //         },
    //         .function_body = {R"STR(
    // vec4 over = in1;
    // vec4 under = in2;
    // return over + (1. - over.a) * under;
    //     )STR"},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name      = "Add (Blend Mode)",
    //         .signature = {
    //             .from  = PrimitiveType::RGBA,
    //             .to    = PrimitiveType::RGBA,
    //             .arity = 2,
    //         },
    //         .function_body = {R"STR(
    // vec4 over = in1;
    // vec4 under = in2;
    // return over + under;
    //     )STR"},
    //         .properties    = {},
    //     });

    //     this->add_node_definition({
    //         .name          = "Mask Image",
    //         .signature     = Signature::ImageRGBA,
    //         .function_body = {R"STR(
    // vec2 uv = in1;
    // return `Image`(uv) * `Mask`(uv);
    //     )STR"},
    //         .inputs        = {
    //             {{.name = "`Image`", .signature = Signature::ImageRGBA}},
    //             {{.name = "`Mask`", .signature = Signature::FloatField}},
    //         },
    //         .properties = {},
    //     });

    //     this->add_node_definition({
    //         .name          = "Curve Renderer",
    //         .signature     = Signature::FloatField,
    //         .function_body = {R"STR(
    // float mask = 0.;
    // for (int i = 0; i < `N`; ++i)
    // {
    //     float t = i / float(`N`);
    //     vec2 center = `Curve`(t);
    //     mask += smoothstep(`Edge Blur`, -`Edge Blur`, length(in1 - center) - `Radius`);
    // }
    // return mask;
    //     )STR"},
    //         .inputs        = {
    //             {{.name = "`Curve`", .signature = Signature::ParametricCurve}},
    //         },
    //         .properties = {
    //             Cool::InputDefinition<int>{"`N`"},
    //             Cool::InputDefinition<float>{"`Radius`"},
    //             Cool::InputDefinition<float>{"`Edge Blur`"},
    //         },
    //     });
}

} // namespace Lab