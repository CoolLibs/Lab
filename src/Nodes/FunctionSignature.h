#pragma once
#include "PrimitiveType.h"

namespace Lab {

struct FunctionSignature {
    PrimitiveType from{};
    PrimitiveType to{};
    size_t        arity{1};

    friend auto operator==(FunctionSignature const&, FunctionSignature const&) -> bool = default;
};

struct FunctionSignatureAsString {
    std::string return_type;
    std::string name;
    std::string arguments_list;
};

// struct ArgumentDesc {
//     std::string   name;
//     PrimitiveType type;
// };

// using ArgumentsList = std::vector<ArgumentDesc>;

// struct FunctionSignatureWithNames {
//     PrimitiveType return_type;
//     ArgumentsList arguments_list;
// };

// struct MainFunctionSignature {
//     FunctionSignature        signature;
//     std::vector<std::string> argument_names;
// };

inline auto to_string(FunctionSignature signature) -> std::string
{
    return fmt::format("{}to{}Arity{}", cpp_type_as_string(signature.from), cpp_type_as_string(signature.to), signature.arity);
}

// auto make_function_signature_with_names(MainFunctionSignature const& signature)
//     -> FunctionSignatureWithNames;

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
