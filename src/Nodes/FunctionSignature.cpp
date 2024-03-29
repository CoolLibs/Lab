#include "FunctionSignature.h"
#include "PrimitiveType.h"

namespace Lab {

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

auto curve_3D_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::Float,
        .to    = PrimitiveType::Vec3,
        .arity = 1,
    };
}
auto is_curve_3D(FunctionSignature signature) -> bool
{
    return signature == curve_3D_signature();
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

auto particle_2D_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::Particle2D,
        .to    = PrimitiveType::Particle2D,
        .arity = 1,
    };
}
auto particle_3D_signature() -> FunctionSignature
{
    return FunctionSignature{
        .from  = PrimitiveType::Particle3D,
        .to    = PrimitiveType::Particle3D,
        .arity = 1,
    };
}
auto is_particle_2D(FunctionSignature signature) -> bool
{
    return signature == particle_2D_signature();
}
auto is_particle_3D(FunctionSignature signature) -> bool
{
    return signature == particle_3D_signature();
}
auto is_particle(FunctionSignature signature) -> bool
{
    return is_particle_2D(signature) || is_particle_3D(signature);
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
