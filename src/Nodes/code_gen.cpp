#include "code_gen.h"
#include <fmt/compile.h>
#include "gen_desired_function_body.h"

namespace Lab {

static auto gen_params(
    FunctionSignature const& signature
) -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR({type} in1)STR"
        ),
        "type"_a = to_glsl(signature.from)
    );
}

static auto gen_function_declaration(
    FunctionSignature const& signature,
    std::string_view         name
) -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR({return_type} {name}({params}))STR"
        ),
        "return_type"_a = to_glsl(signature.to),
        "name"_a        = name,
        "params"_a      = gen_params(signature)
    );
}

struct gen_function__params {
    FunctionSignature const& signature;
    std::string_view         name;
    GlslCode const&          body;
};

static auto gen_function(gen_function__params p)
    -> GlslCode
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
        "body"_a        = p.body.data
    )};
}

static auto is_not_alphanumeric(char c) -> bool
{
    return !(
        ('0' <= c && c <= '9')
        || ('a' <= c && c <= 'z')
        || ('A' <= c && c <= 'Z')
    );
}

/// Returns a string with only letters and numbers, no underscores.
/// This string does not start with a number.
static auto valid_glsl(std::string s)
    -> std::string
{
    // For glsl variable name rules, see https://www.informit.com/articles/article.aspx?p=2731929&seqNum=3, section "Declaring Variables".
    s.erase(std::remove_if(s.begin(), s.end(), &is_not_alphanumeric), s.end()); // `s` can only contain letters and numbers (and _, but two consecutive underscores is invalid so we don't allow any: this is the simplest way to enforce that rule, at the cost of slightly uglier names)
    return "node_" + s;                                                         // We need a prefix to make sure `s` does not start with a number.
}

static auto base_function_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id
) -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR({name}_{id})STR"
        ),
        "name"_a = valid_glsl(definition.name()), // NB: We don't have to worry about the unicity of that name because we append an ID anyway
        "id"_a   = to_string(id.underlying_uuid())
    );
}

auto gen_base_function(
    NodeDefinition const& definition,
    Cool::NodeId const&   id
) -> GlslCode
{
    return gen_function({
        .signature = definition.signature,
        .name      = base_function_name(definition, id),
        .body      = definition.function_body,
    });
}

static auto gen_desired_body(
    Node const& node,
    Cool::NodeId const& /*id*/,
    FunctionSignature const&                    desired_signature,
    std::string_view                            name,
    std::string_view                            input_function_name,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition
)
    -> std::optional<GlslCode>
{
    const NodeDefinition* def = get_node_definition(node.definition_name);
    if (!def)
        return std::nullopt;

    return std::visit(
        [&](auto&& current_from, auto&& current_to,
            auto&& desired_from, auto&& desired_to) { return gen_desired_function_body(
                                                          current_from, current_to,
                                                          desired_from, desired_to,
                                                          name, input_function_name
                                                      ); },
        def->signature.from, def->signature.to,
        desired_signature.from, desired_signature.to
    );
}

auto gen_desired_function(
    Node const&                                 node,
    Cool::NodeId const&                         id,
    FunctionSignature const&                    desired_signature,
    std::string_view                            name,
    std::string_view                            input_function_name,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition
) -> std::optional<GlslCode>
{
    const auto body = gen_desired_body(node, id, desired_signature, name, input_function_name, get_node_definition);
    if (!body)
        return std::nullopt;

    return gen_function({
        .signature = desired_signature,
        .name      = name,
        .body      = *body,
    });
}

} // namespace Lab