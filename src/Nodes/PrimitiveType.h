#pragma once

namespace Lab {

namespace internal {
template<typename T, typename... U>
concept IsAnyOf = (std::same_as<T, U> || ...);
}

namespace PrimitiveType {
#include "generated/primitive_types_structs_definitions.inl"
} // namespace PrimitiveType

template<typename T>
concept PrimitiveTypeC = internal::IsAnyOf<T,
#include "generated/primitive_types_list.inl"
                                           >;
using AnyPrimitiveType = std::variant<
#include "generated/primitive_types_list.inl"
    >;

template<PrimitiveTypeC T>
auto glsl_type_as_string(T = {}) -> std::string
{
#include "generated/glsl_type_as_string.inl"
}

inline auto glsl_type_as_string(AnyPrimitiveType type) -> std::string
{
    return std::visit([](auto&& type) { return glsl_type_as_string(type); }, type);
}

template<PrimitiveTypeC T>
auto cpp_type_as_string(T = {}) -> std::string
{
#include "generated/cpp_type_as_string.inl"
}

inline auto cpp_type_as_string(AnyPrimitiveType type) -> std::string
{
    return std::visit([](auto&& type) { return cpp_type_as_string(type); }, type);
}

} // namespace Lab