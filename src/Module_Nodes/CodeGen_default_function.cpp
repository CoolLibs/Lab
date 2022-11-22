#include "CodeGen_default_function.h"

namespace Lab {

auto gen_default_function(FunctionSignature signature, AlreadyGeneratedFunctions& already_generated_functions)
    -> tl::expected<Function, std::string>
{
    if (signature == FunctionSignature{.from = PrimitiveType::UV, .to = PrimitiveType::Color})
    {
        return Function{{
                            .name           = "default_image",
                            .implementation = R"STR(
vec3 default_image(vec2 uv)
{
    return vec3(uv, 0.);
}
)STR",
                        },
                        already_generated_functions};
    }

    if (signature == FunctionSignature{.from = PrimitiveType::Float, .to = PrimitiveType::Color})
    {
        return Function{{
                            .name           = "default_colorizer",
                            .implementation = R"STR(
vec3 default_colorizer(float x)
{
    return vec3(x);
}
)STR",
                        },
                        already_generated_functions};
    }

    if (signature.to == PrimitiveType::Void)
    {
        auto const name = fmt::format("default_{}_to_void", cpp_type_as_string(signature.from));
        return Function{{
                            .name           = name,
                            .implementation = fmt::format(R"STR(
int {}({} unused_parameter)
{{
    return 0;
}}
)STR",
                                                          name, glsl_type_as_string(signature.from)),
                        },
                        already_generated_functions};
    }

    if (signature == FunctionSignature{.from = PrimitiveType::UV, .to = PrimitiveType::Float})
    {
        return Function{{
                            .name           = "default_uv_to_float",
                            .implementation = R"STR(
float default_uv_to_float(vec2 uv)
{
    return uv.x * 0.5 / _aspect_ratio + 0.5;
}
)STR",
                        },
                        already_generated_functions};
    }

    return tl::make_unexpected(fmt::format(
        "Could not generate a default function from {} to {}.",
        cpp_type_as_string(signature.from), cpp_type_as_string(signature.to)
    ));
}

} // namespace Lab
