#include "gen_desired_function_implementation.h"
#include "gen_default_function.h"

namespace Lab {

// A->A  ~~>  A->A
static auto all_equal(FunctionSignature current, FunctionSignature desired) -> bool
{
    return current.from == current.to && current == desired;
}

// A->B  ~~>  A->B
static auto same_signature(FunctionSignature current, FunctionSignature desired) -> bool
{
    return current == desired;
}

// A->A  ~~>  A->B
static auto transforming_input(FunctionSignature current, FunctionSignature desired) -> bool
{
    return current.from == current.to && current.to == desired.from;
}

// B->B  ~~>  A->B
static auto transforming_output(FunctionSignature current, FunctionSignature desired) -> bool
{
    return current.from == current.to && current.from == desired.to;
}

// A->B  ~~>  A->C
static auto converting_output(FunctionSignature current, FunctionSignature desired) -> bool
{
    return current.from == desired.from && current.to != desired.to;
}

// A->B  ~~>  C->B
static auto converting_input(FunctionSignature current, FunctionSignature desired) -> bool
{
    return current.to == desired.to && current.from != desired.from;
}

// TODO(JF) test all of these, to make sure overload resolution doesn't change when we add options

auto input_function_desired_signature(
    FunctionSignature current,
    FunctionSignature desired
) -> std::optional<FunctionSignature>
{
    // A->A  ~~>  A->A
    if (all_equal(current, desired))
    {
        return std::nullopt;
    }

    // A->B  ~~>  A->B
    if (same_signature(current, desired))
    {
        return std::nullopt;
    }

    // A->A  ~~>  A->B
    if (transforming_input(current, desired))
    {
        return desired;
    }

    // B->B  ~~>  A->B
    if (transforming_output(current, desired))
    {
        return desired;
    }

    // A->B  ~~>  A->C
    if (converting_output(current, desired))
    {
        return FunctionSignature{
            .from = current.to,
            .to   = desired.to,
        };
    }

    // A->B  ~~>  C->B
    if (converting_input(current, desired))
    {
        return FunctionSignature{
            .from = desired.from,
            .to   = current.from,
        };
    }

    // Catch all case
    return FunctionSignature{
        .from = desired.from,
        .to   = current.from,
    };
}
auto gen_desired_function_implementation(
    FunctionSignature          current,
    FunctionSignature          desired,
    std::string_view           base_function_name,
    std::string_view           input_function_name,
    AlreadyGeneratedFunctions& already_generated_functions
) -> FunctionImplementation
{
    using namespace fmt::literals;

    // A->A  ~~>  A->A
    if (all_equal(current, desired))
    {
        return {
            .before_function = "",
            .function_body   = fmt::format(
                FMT_COMPILE(
                    R"STR(return {base_function_name}(in1);)STR"
                ),
                "base_function_name"_a = base_function_name
            ),
        };
    }

    // A->B  ~~>  A->B
    if (same_signature(current, desired))
    {
        return {
            .before_function = "",
            .function_body   = fmt::format(
                FMT_COMPILE(
                    R"STR(return {base_function_name}(in1);)STR"
                ),
                "base_function_name"_a = base_function_name
            ),
        };
    }

    // A->A  ~~>  A->B
    if (transforming_input(current, desired))
    {
        return {
            .before_function = "",
            .function_body   = fmt::format(
                FMT_COMPILE(
                    R"STR(return {input_function_name}({base_function_name}(in1));)STR"
                ),
                "base_function_name"_a  = base_function_name,
                "input_function_name"_a = input_function_name
            ),
        };
    }

    // B->B  ~~>  A->B
    if (transforming_output(current, desired))
    {
        return {
            .before_function = "",
            .function_body   = fmt::format(
                FMT_COMPILE(
                    R"STR(return {base_function_name}({input_function_name}(in1));)STR"
                ),
                "base_function_name"_a  = base_function_name,
                "input_function_name"_a = input_function_name
            ),
        };
    }

    // A->B  ~~>  A->C
    if (converting_output(current, desired))
    {
        return {
            .before_function = "",
            .function_body   = fmt::format(
                FMT_COMPILE(
                    R"STR(return {input_function_name}({base_function_name}(in1));)STR"
                ),
                "base_function_name"_a  = base_function_name,
                "input_function_name"_a = input_function_name
            ),
        };
    }

    // A->B  ~~>  C->B
    if (converting_input(current, desired))
    {
        return {
            .before_function = "",
            .function_body   = fmt::format(
                FMT_COMPILE(
                    R"STR(return {base_function_name}({input_function_name}(in1));)STR"
                ),
                "base_function_name"_a  = base_function_name,
                "input_function_name"_a = input_function_name
            ),
        };
    }

    // Catch all case
    const auto default_function = gen_default_function({.from = current.to, .to = desired.to}, already_generated_functions);
    return {
        .before_function = default_function.definition,
        .function_body   = fmt::format(
            FMT_COMPILE(
                R"STR(return {default_function_name}({base_function_name}({input_function_name}(in1)));)STR"
            ),
            "default_function_name"_a = default_function.name,
            "base_function_name"_a    = base_function_name,
            "input_function_name"_a   = input_function_name
        ),
    };
}

} // namespace Lab