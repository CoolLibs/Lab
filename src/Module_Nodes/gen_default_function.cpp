#include "gen_default_function.h"

namespace Lab {

auto gen_default_function(FunctionSignature signature, AlreadyGeneratedFunctions& already_generated_functions) -> Function
{
    if (signature == FunctionSignature{.from = PrimitiveType::UV{}, .to = PrimitiveType::Color{}})
    {
        return Function{{
                            .name       = "default_image",
                            .definition = R"STR(
vec3 default_image(vec2 uv)
{
    return vec3(uv, 0.);
}
)STR",
                        },
                        already_generated_functions};
    }
    if (signature == FunctionSignature{.from = PrimitiveType::Float{}, .to = PrimitiveType::Color{}})
    {
        return Function{{
                            .name       = "default_colorizer",
                            .definition = R"STR(
vec3 default_colorizer(float x)
{
    return vec3(x);
}
)STR",
                        },
                        already_generated_functions};
    }

    if (signature.to == AnyPrimitiveType{PrimitiveType::Void{}})
    {
        auto const name = fmt::format("default_{}_to_void", cpp_type_as_string(signature.from));
        return Function{{
                            .name       = name,
                            .definition = fmt::format(R"STR(
int {}({})
{{
    return 0;
}}
)STR",
                                                      name, glsl_type_as_string(signature.from)),
                        },
                        already_generated_functions};
    }

    return Function{{
                        .name       = "ERROR_NO_DEFAULT_FUNCTION",
                        .definition = fmt::format("ERROR: Could not generate a default function from {} to {}", cpp_type_as_string(signature.from), cpp_type_as_string(signature.to)),
                    },
                    already_generated_functions};
}

} // namespace Lab
