#pragma once

namespace Lab {

/// Catch all, error case if there is no better overload match
template<PrimitiveTypeC A, PrimitiveTypeC B, PrimitiveTypeC C, PrimitiveTypeC D>
auto input_function_desired_signature(
    A /*current_from*/, B /*current_to*/,
    C /*desired_from*/, D /*desired_to*/
) -> std::optional<FunctionSignature>
{
    return std::nullopt;
}
template<PrimitiveTypeC A, PrimitiveTypeC B, PrimitiveTypeC C, PrimitiveTypeC D>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    C /*desired_from*/, D /*desired_to*/,
    std::string_view /*base_function_name*/,
    std::string_view /*input_function_name*/
) -> std::string
{
    return {fmt::format(
        "[gen_desired_function_body()] Error: No known conversion from '{}->{}' to '{}->{}'.",
        cpp_type_as_string<A>(),
        cpp_type_as_string<B>(),
        cpp_type_as_string<C>(),
        cpp_type_as_string<D>()
    )};
}

template<PrimitiveTypeC A>
auto input_function_desired_signature(
    A /*current_from*/, A /*current_to*/,
    A /*desired_from*/, A /*desired_to*/
) -> std::optional<FunctionSignature>
{
    return std::nullopt;
}
template<PrimitiveTypeC A>
auto gen_desired_function_body(
    A /*current_from*/, A /*current_to*/,
    A /*desired_from*/, A /*desired_to*/,
    std::string_view base_function_name,
    std::string_view /*input_function_name*/
) -> std::string
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {base_function_name}(in1);)STR"
        ),
        "base_function_name"_a = base_function_name
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto input_function_desired_signature(
    A /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/
) -> std::optional<FunctionSignature>
{
    return std::nullopt;
}
template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view base_function_name,
    std::string_view /*input_function_name*/
) -> std::string
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {base_function_name}(in1);)STR"
        ),
        "base_function_name"_a = base_function_name
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto input_function_desired_signature(
    A /*current_from*/, A /*current_to*/,
    A /*desired_from*/, B /*desired_to*/
) -> std::optional<FunctionSignature>
{
    return FunctionSignature{
        .from = A{},
        .to   = B{},
    };
}
template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    A /*current_from*/, A /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view base_function_name,
    std::string_view input_function_name
) -> std::string
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {input_function_name}({base_function_name}(in1));)STR"
        ),
        "base_function_name"_a  = base_function_name,
        "input_function_name"_a = input_function_name
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto input_function_desired_signature(
    B /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/
) -> std::optional<FunctionSignature>
{
    return FunctionSignature{
        .from = A{},
        .to   = B{},
    };
}
template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    B /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view base_function_name,
    std::string_view input_function_name
) -> std::string
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {base_function_name}({input_function_name}(in1));)STR"
        ),
        "base_function_name"_a  = base_function_name,
        "input_function_name"_a = input_function_name
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B, PrimitiveTypeC C>
auto input_function_desired_signature(
    A /*current_from*/, B /*current_to*/,
    A /*desired_from*/, C /*desired_to*/
) -> std::optional<FunctionSignature>
{
    return FunctionSignature{
        .from = B{},
        .to   = C{},
    };
}
template<PrimitiveTypeC A, PrimitiveTypeC B, PrimitiveTypeC C>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    A /*desired_from*/, C /*desired_to*/,
    std::string_view base_function_name,
    std::string_view input_function_name
) -> std::string
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {input_function_name}({base_function_name}(in1));)STR"
        ),
        "base_function_name"_a  = base_function_name,
        "input_function_name"_a = input_function_name
    )};
}

} // namespace Lab

// TODO test all of these, to make sure overload resolution doesn't change when we add options