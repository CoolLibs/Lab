#include "FunctionSignature.h"
#include <Module_Nodes/FunctionSignature.h>
#include <Module_Nodes/PrimitiveType.h>
#include "can_convert.h"

namespace Lab {

auto make_complete_function_signature(MainFunctionSignature const& signature)
    -> CompleteFunctionSignature
{
    auto res = CompleteFunctionSignature{};

    res.output_type = signature.signature.to;

    std::transform(
        signature.parameter_names.begin(), signature.parameter_names.end(),
        std::back_inserter(res.parameters),
        [&](std::string const& name) {
            return ParamDesc{
                .name = name,
                .type = signature.signature.from,
            };
        }
    );

    return res;
}

auto curve_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::Float,
        .to    = PrimitiveType::UV,
        .arity = 1,
    };
}
auto is_curve(FunctionSignature signature) -> bool
{
    return signature == curve_signature();
}

auto shape_2D_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::UV,
        .to    = PrimitiveType::SignedDistance,
        .arity = 1,
    };
}
auto is_shape_2D(FunctionSignature signature) -> bool
{
    return signature == shape_2D_signature();
}

auto shape_3D_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::Vec3,
        .to    = PrimitiveType::SignedDistance,
        .arity = 1,
    };
}
auto is_shape_3D(FunctionSignature signature) -> bool
{
    return signature == shape_3D_signature();
}

auto is_image(FunctionSignature signature) -> bool
{
    return is_color_type(signature.to);
}

auto is_greyscale(FunctionSignature signature) -> bool
{
    return can_convert(signature.to, PrimitiveType::Float)
           || is_greyscale_type(signature.to);
}

auto modifier_2D_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::UV,
        .to    = PrimitiveType::UV,
        .arity = 1,
    };
}
auto is_modifier_2D(FunctionSignature signature) -> bool
{
    return signature == modifier_2D_signature();
}

auto is_color_or_greyscale_modifier(FunctionSignature signature) -> bool
{
    return signature.arity == 1
           && is_color_or_greyscale_type(signature.from)
           && is_color_or_greyscale_type(signature.to);
}

} // namespace Lab