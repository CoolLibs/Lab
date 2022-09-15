#include "code_gen.h"
#include <fmt/compile.h>

namespace Lab {

static auto gen_function_declaration(
    FunctionSignature const& signature,
    std::string_view         name
)
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR({return_type} {name}({params}))STR"
        ),
        "return_type"_a = to_string(signature.to),
        "name"_a        = name,
        "params"_a =
    )};
}

struct gen_function__params {
    FunctionSignature const& signature;
    std::string_view         name;
    GlslCode const&          body;
};

static auto gen_function(gen_function__params p)
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR({declaration}
{{
    {body}
}}
)STR"
        ),
        "declaration"_a = gen_function_declaration(p.signature, p.name),
        "body"_a        = p.body
    )};
}

static auto is_not_alphanumeric(char c) -> bool
{
    return !(
        ('0' <= c && c <= '9') ||
        ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z')
    );
}

/// Returns a string with only letters and numbers, no underscores.
/// This string does not start with a number.
static auto valid_glsl(std::string s) -> std::string
{
    // For glsl variable name rules, see https://www.informit.com/articles/article.aspx?p=2731929&seqNum=3, section "Declaring Variables".
    s.erase(std::remove_if(s.begin(), s.end(), &is_not_alphanumeric), s.end()); // `s` can only contain letters and numbers (and _, but two consecutive underscores is invalid so we don't allow any: this is the simplest way to enforce that rule, at the cost of slightly uglier names)
    return "node_" + s;                                                         // We need a prefix to make sure `s` does not start with a number.
}

static auto base_function_name(
    NodeDefinition const& definition,
    NodeID const&         id
) -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR({name}_{id})STR"
        ),
        "name"_a = valid_glsl(definition.name), // NB: We don't have to worry about the unicity of that name because we append an ID anyway
        "id"_a   = id.underlying_uuid()
    );
}

auto gen_base_function(
    NodeDefinition const& definition,
    NodeID const&         id
) -> GlslCode
{
    return gen_function({
        .signature = definition.signature,
        .name      = base_function_name(definition, id),
        .body      = definition.function_body,
    });
}

static auto gen_desired_body(
    Node const&              node,
    NodeID const&            id,
    FunctionSignature const& desired_signature,
) -> GlslCode
{
    using namespace fmt::literals;
    return {fmt::format(
        FMT_COMPILE(
            R"STR(
)STR"
        ),
        // "declaration"_a = gen_function_declaration(signature, name),
        // "body"_a        = body
    )};
}

auto gen_desired_function(
    Node const&              node,
    NodeID const&            id,
    FunctionSignature const& desired_signature,
    std::string_view         name
) -> GlslCode
{
    return gen_function({
        .signature = desired_signature,
        .name      = name,
        .body      = gen_desired_body(node, id, desired_signature),
    });
}

} // namespace Lab