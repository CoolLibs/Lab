#pragma once

namespace Lab {

/// Catch all, error case if there is no better overload match
template<PrimitiveTypeC A, PrimitiveTypeC B, PrimitiveTypeC C, PrimitiveTypeC D>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    C /*desired_from*/, D /*desired_to*/,
    std::string_view /*function_name*/,
    std::string_view /*input_function_name*/
) -> GlslCode
{
    return {fmt::format(
        "[gen_desired_function_body()] Error: No known conversion from '{}->{}' to '{}->{}'.",
        cpp_type_as_string<A>(),
        cpp_type_as_string<B>(),
        cpp_type_as_string<C>(),
        cpp_type_as_string<D>()
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view function_name,
    std::string_view /*input_function_name*/
) -> GlslCode
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {function_name}(in1);)STR"
        ),
        "function_name"_a = function_name
    )};
}

template<PrimitiveTypeC A>
auto gen_desired_function_body(
    A /*current_from*/, A /*current_to*/,
    A /*desired_from*/, A /*desired_to*/,
    std::string_view function_name,
    std::string_view /*input_function_name*/
) -> GlslCode
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {function_name}(in1);)STR"
        ),
        "function_name"_a = function_name
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    A /*current_from*/, A /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view function_name,
    std::string_view input_function_name
) -> GlslCode
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {input_function_name}({function_name}(in1));)STR"
        ),
        "function_name"_a       = function_name,
        "input_function_name"_a = input_function_name
    )};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    B /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view function_name,
    std::string_view input_function_name
) -> GlslCode
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(return {function_name}({input_function_name}(in1));)STR"
        ),
        "function_name"_a       = function_name,
        "input_function_name"_a = input_function_name
    )};
}

} // namespace Lab

// TODO test all of these, to make sure overload resolution doesn't change when we add options