#include "CodeGen_default_function.h"

namespace Lab {

auto gen_default_function(FunctionSignature signature, CodeGenContext& context)
    -> ExpectedFunctionName
{
    if (signature == Signature::Image)
    {
        return context.push_function({
            .name           = "default_image",
            .implementation = R"STR(
vec3 default_image(vec2 uv)
{
    return vec3(uv, 0.);
}
)STR",
        });
    }

    if (signature == Signature::Colorizer)
    {
        return context.push_function({
            .name           = "default_colorizer",
            .implementation = R"STR(
vec3 default_colorizer(float x)
{
    return vec3(x);
}
)STR",
        });
    }

    if (signature.to == PrimitiveType::Void)
    {
        auto const name = fmt::format("default_", to_string(signature));
        return context.push_function({
            .name           = name, // TODO(JF) Handle arity:
            .implementation = fmt::format(R"STR(
int {}({} unused_parameter)
{{
    return 0;
}}
)STR",
                                          name, glsl_type_as_string(signature.from)),
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

    return tl::make_unexpected(fmt::format(
        "Could not generate a default function from {} to {}.",
        cpp_type_as_string(signature.from) /* TODO(JF) Write this type as many times as the arity */, cpp_type_as_string(signature.to), signature.arity
    ));
}

} // namespace Lab
