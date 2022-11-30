#include "CodeGen_default_function.h"
#include "Module_Nodes/FunctionSignature.h"
#include "Module_Nodes/PrimitiveType.h"
#include "fmt/compile.h"

namespace Lab {

static auto comma_separated(std::string const& str, size_t count)
    -> std::string
{
    std::string res;

    for (size_t i = 0; i < count; ++i)
    {
        res += str;
        if (i != count - 1)
            res += ", ";
    }

    if (count == 1)
        return res;
    else
        return fmt::format("({})", res);
}

auto gen_default_function(FunctionSignature signature, CodeGenContext& context)
    -> ExpectedFunctionName
{
    using fmt::literals::operator""_a;

    if (signature == Signature::ImageRGB)
    {
        return context.push_function({
            .name           = "default_image_rgb",
            .implementation = R"STR(
vec3 default_image_rgb/*coollabdef*/(vec2 uv)
{
    return vec3(uv, 0.);
}
)STR",
        });
    }

    if (signature == Signature::ImageRGBA)
    {
        return context.push_function({
            .name           = "default_image_rgba",
            .implementation = R"STR(
vec4 default_image_rgba/*coollabdef*/(vec2 uv)
{
    return vec4(uv, 0., 1.);
}
)STR",
        });
    }

    if (signature == Signature::ColorizerRGB)
    {
        return context.push_function({
            .name           = "default_colorizer_rgb",
            .implementation = R"STR(
vec3 default_colorizer_rgb/*coollabdef*/(float x)
{
    return vec3(x);
}
)STR",
        });
    }

    if (signature == Signature::ColorizerRGBA)
    {
        return context.push_function({
            .name           = "default_colorizer_rgba",
            .implementation = R"STR(
vec4 default_colorizer_rgba/*coollabdef*/(float x)
{
    return vec4(x);
}
)STR",
        });
    }

    if (signature == FunctionSignature{.from = PrimitiveType::RGB, .to = PrimitiveType::RGBA})
    {
        return context.push_function({
            .name           = "default_rgb_to_rgba",
            .implementation = R"STR(
vec4 default_rgb_to_rgba/*coollabdef*/(vec3 rgb)
{
    return vec4(rgb, 1.);
}
)STR",
        });
    }

    if (signature == FunctionSignature{.from = PrimitiveType::RGBA, .to = PrimitiveType::RGB})
    {
        return context.push_function({
            .name           = "default_rgba_to_rgb",
            .implementation = R"STR(
vec3 default_rgba_to_rgb/*coollabdef*/(vec4 rgba)
{
    return rgba.rgb;
}
)STR",
        });
    }

    if (signature == Signature::FloatField)
    {
        return context.push_function({
            .name           = "default_uv_to_float",
            .implementation = R"STR(
float default_uv_to_float/*coollabdef*/(vec2 uv)
{
    return uv.x * 0.5 / _aspect_ratio + 0.5;
}
)STR",
        });
    }

    if (signature == Signature::ParametricCurve)
    {
        return context.push_function({
            .name           = "default_curve",
            .implementation = R"STR(
vec2 default_curve/*coollabdef*/(float t)
{
    float angle = TAU * t;
    return vec2(cos(angle), sin(angle));
}
)STR",
        });
    }

    if (signature == Signature::BlendMode)
    {
        return context.push_function({
            .name           = "default_blend_mode",
            .implementation = R"STR(
vec4 default_blend_mode/*coollabdef*/(vec4 over, vec4 under)
{
    // This is a over (aka Normal Blend Mode). We assume premultiplied alpha.
    return over + (1. - over.a) * under;
}
)STR",
        });
    }

    if (signature.from == PrimitiveType::Void)
    {
        if (signature.to == PrimitiveType::UV) // Special case for UVs; they are not really constant, they use the current uv map, to which we have applied all the uv transformations, starting from normalized_uv().
        {
            return context.push_function({
                .name           = "default_uv",
                .implementation = R"STR(
vec2 default_uv/*coollabdef*/()
{
    return coollab_context.uv;
}
)STR",
            });
        }
        auto const glsl_type = raw_glsl_type_as_string(signature.to);
        auto const name      = fmt::format("default_constant_{}", glsl_type);
        return context.push_function({
            .name           = name,
            .implementation = fmt::format(
                FMT_COMPILE(R"STR(
{type} {name}/*coollabdef*/()
{{
    return {type}(0);
}}
)STR"),
                "type"_a = glsl_type,
                "name"_a = name
            ),
        });
    }

    return tl::make_unexpected(fmt::format(
        "Could not generate a default function from {} to {}.",
        comma_separated(cpp_type_as_string(signature.from), signature.arity), cpp_type_as_string(signature.to), signature.arity
    ));
}

} // namespace Lab
