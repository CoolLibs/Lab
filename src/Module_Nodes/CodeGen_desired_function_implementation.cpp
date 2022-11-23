#include "CodeGen_desired_function_implementation.h"

namespace Lab {

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

static auto gen_args(size_t arity) -> std::string
{
    std::string res{};

    for (size_t i = 0; i < arity; ++i)
    {
        res += fmt::format("in{}", i + 1);
        if (i != arity - 1)
            res += ", ";
    }

    return res;
}

auto gen_desired_function_implementation(
    FunctionSignature            current,
    FunctionSignature            desired,
    size_t                       current_signature_arity,
    std::string_view             base_function_name,
    InputFunctionGenerator_Ref   gen_input_function,
    DefaultFunctionGenerator_Ref gen_default_function
) -> tl::expected<std::string, std::string>
{
    using namespace fmt::literals;

    // A->B  ~~>  A->B
    if (same_signature(current, desired))
    {
        return fmt::format(
            FMT_COMPILE(
                "return {base_function_name}({args});"
            ),
            "base_function_name"_a = base_function_name,
            "args"_a               = gen_args(current_signature_arity)
        );
    }

    // A->A  ~~>  A->B
    if (transforming_input(current, desired))
    {
        auto const input_func_name = gen_input_function(desired);
        if (!input_func_name)
            return tl::make_unexpected(input_func_name.error());

        return fmt::format(
            FMT_COMPILE(
                "return {input_function_name}({base_function_name}({args}));"
            ),
            "base_function_name"_a  = base_function_name,
            "input_function_name"_a = *input_func_name,
            "args"_a                = gen_args(current_signature_arity)
        );
    }

    // B->B  ~~>  A->B
    if (transforming_output(current, desired))
    {
        auto const input_func_name = gen_input_function(desired);
        if (!input_func_name)
            return tl::make_unexpected(input_func_name.error());

        return fmt::format(
            FMT_COMPILE(
                "return {base_function_name}({input_function_name}({args}));"
            ),
            "base_function_name"_a  = base_function_name,
            "input_function_name"_a = *input_func_name,
            "args"_a                = gen_args(current_signature_arity)
        );
    }

    // A->B  ~~>  A->C
    if (converting_output(current, desired))
    {
        auto const input_func_name = gen_input_function({.from = current.to, .to = desired.to});
        if (!input_func_name)
            return tl::make_unexpected(input_func_name.error());

        return fmt::format(
            FMT_COMPILE(
                "return {input_function_name}({base_function_name}({args}));"
            ),
            "base_function_name"_a  = base_function_name,
            "input_function_name"_a = *input_func_name,
            "args"_a                = gen_args(current_signature_arity)
        );
    }

    // A->B  ~~>  C->B
    if (converting_input(current, desired))
    {
        auto const input_func_name = gen_input_function({.from = desired.from, .to = current.from});
        if (!input_func_name)
            return tl::make_unexpected(input_func_name.error());

        return fmt::format(
            FMT_COMPILE(
                "return {base_function_name}({input_function_name}({args}));"
            ),
            "base_function_name"_a  = base_function_name,
            "input_function_name"_a = *input_func_name,
            "args"_a                = gen_args(current_signature_arity)
        );
    }

    // Catch all case
    auto const default_func_name = gen_default_function({.from = current.to, .to = desired.to});
    if (!default_func_name)
        return tl::make_unexpected(default_func_name.error());

    auto const input_func_name = gen_input_function({.from = desired.from, .to = current.from});
    if (!input_func_name)
        return tl::make_unexpected(input_func_name.error());

    return fmt::format(
        FMT_COMPILE(
            "return {default_function_name}({base_function_name}({input_function_name}({args})));"
        ),
        "default_function_name"_a = *default_func_name,
        "base_function_name"_a    = base_function_name,
        "input_function_name"_a   = *input_func_name,
        "args"_a                  = gen_args(current_signature_arity)
    );
}

} // namespace Lab