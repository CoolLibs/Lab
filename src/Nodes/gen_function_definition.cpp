#include "gen_function_definition.h"

namespace Lab {

static auto gen_function_declaration(FunctionSignatureAsString const& signature, std::string const& unique_name) -> std::string
{
    using fmt::literals::operator""_a;
    return fmt::format(
        FMT_COMPILE(
            R"STR({return_type} {name}/*needs_coollab_context*/({args}))STR"
        ),
        "return_type"_a = signature.return_type,
        "name"_a        = unique_name, // We don't use signature.name because we need to use a unique name that has been generated for this instance of the function
        "args"_a        = signature.arguments_list
    );
}

auto gen_function_definition(Params_gen_function_definition const& p)
    -> std::string
{
    using fmt::literals::operator""_a;
    return {fmt::format(
        FMT_COMPILE(
            R"STR({before_function}
            
{declaration}
{{
    {body}
}}
)STR"
        ),
        "before_function"_a = p.before_function,
        "declaration"_a     = gen_function_declaration(p.signature_as_string, p.unique_name),
        "body"_a            = p.body
    )};
}

} // namespace Lab