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
auto to_glsl(T) -> std::string
{
#include "generated/to_glsl.inl"
}

inline auto to_glsl(AnyPrimitiveType type) -> std::string
{
    return std::visit([](auto&& type) { return to_glsl(type); }, type);
}

} // namespace Lab