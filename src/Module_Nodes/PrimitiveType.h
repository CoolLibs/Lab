#pragma once

namespace Lab {

enum class PrimitiveType {
#include "generated/primitive_types_enum_members.inl"
};

/// Returns the glsl type, e.g. "float", "vec3", etc.
auto raw_glsl_type_as_string(PrimitiveType) -> std::string;
/// Returns the glsl type and a comment indicating the semantic type, e.g. "/* Float */ float", "/* RGB */ vec3", etc.
auto glsl_type_as_string(PrimitiveType) -> std::string;
/// Returns the PrimitiveType as a string
auto cpp_type_as_string(PrimitiveType) -> std::string;

auto is_color_type(PrimitiveType) -> bool;
auto is_greyscale_type(PrimitiveType) -> bool;
auto is_color_or_greyscale_type(PrimitiveType) -> bool;

} // namespace Lab