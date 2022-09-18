#pragma once

namespace Lab {

/// Catch all, error case if there is no better overload match
template<PrimitiveTypeC A, PrimitiveTypeC B, PrimitiveTypeC C, PrimitiveTypeC D>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    C /*desired_from*/, D /*desired_to*/,
    std::string_view /*function_name*/
) -> GlslCode
{
    return {"[gen_desired_function_body()] ERROR NO KNOWN CONVERSION"};
}

template<PrimitiveTypeC A, PrimitiveTypeC B>
auto gen_desired_function_body(
    A /*current_from*/, B /*current_to*/,
    A /*desired_from*/, B /*desired_to*/,
    std::string_view function_name
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

} // namespace Lab

// TODO test all of these, to make sure overload resolution doesn't change when we add options