#pragma once
#include "PrimitiveType.h"

namespace Lab {

struct FunctionSignature {
    PrimitiveType from{};
    PrimitiveType to{};
    size_t        arity{1};

    friend auto operator==(FunctionSignature const&, FunctionSignature const&) -> bool = default;
};

inline auto to_string(FunctionSignature signature) -> std::string
{
    return fmt::format("{}to{}Arity{}", cpp_type_as_string(signature.from), cpp_type_as_string(signature.to), signature.arity);
}

auto curve_signature() -> FunctionSignature;
auto is_curve(FunctionSignature) -> bool;

auto curve_3D_signature() -> FunctionSignature;
auto is_curve_3D(FunctionSignature) -> bool;

auto shape_2D_signature() -> FunctionSignature;
auto is_shape_2D(FunctionSignature) -> bool;

auto shape_3D_signature() -> FunctionSignature;
auto is_shape_3D(FunctionSignature) -> bool;

auto particle_2D_signature() -> FunctionSignature;
auto particle_3D_signature() -> FunctionSignature;
auto is_particle_2D(FunctionSignature) -> bool;
auto is_particle_3D(FunctionSignature) -> bool;
auto is_particle(FunctionSignature) -> bool;

auto is_image(FunctionSignature) -> bool;

auto is_greyscale(FunctionSignature) -> bool;

auto modifier_2D_signature() -> FunctionSignature;
auto is_modifier_2D(FunctionSignature) -> bool;

auto is_color_or_greyscale_modifier(FunctionSignature) -> bool;

} // namespace Lab
