#pragma once

namespace Lab {

enum class PrimitiveType {
#include "generated/primitive_types_enum_members.inl"
};

auto glsl_type_as_string(PrimitiveType type) -> std::string;
auto cpp_type_as_string(PrimitiveType type) -> std::string;

} // namespace Lab