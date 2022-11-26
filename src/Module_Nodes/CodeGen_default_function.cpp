#include "CodeGen_default_function.h"

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
    if (signature == Signature::ImageRGB)
    {
        return context.push_function({
            .name           = "default_image_rgb",
            .implementation = R"STR(
vec3 default_image_rgb(vec2 uv)
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
vec4 default_image_rgba(vec2 uv)
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
vec3 default_colorizer_rgb(float x)
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
vec4 default_colorizer_rgba(float x)
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
vec4 default_rgb_to_rgba(vec3 rgb)
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
vec3 default_rgba_to_rgb(vec4 rgba)
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
float default_uv_to_float(vec2 uv)
{
    return uv.x * 0.5 / _aspect_ratio + 0.5;
}
)STR",
        });
    }

    if (signature == Signature::BlendMode)
    {
        return context.push_function({
            .name           = "default_blend_mode",
            .implementation = R"STR(
vec4 default_blend_mode(vec4 over, vec4 under)
{
    // This is a over (aka Normal Blend Mode). We assume premultiplied alpha.
    return over + (1. - over.a) * under;
}
)STR",
        });
    }

    return tl::make_unexpected(fmt::format(
        "Could not generate a default function from {} to {}.",
        comma_separated(cpp_type_as_string(signature.from), signature.arity), cpp_type_as_string(signature.to), signature.arity
    ));
}

} // namespace Lab
