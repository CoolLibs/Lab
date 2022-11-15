#include "generate_shader_code.h"
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include <fmt/compile.h>
#include "Function.h"
#include "FunctionSignature.h"
#include "Node.h"
#include "NodeDefinition.h"
#include "gen_default_function.h"
#include "gen_desired_function_implementation.h"

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
        "type"_a = glsl_type_as_string(signature.from)
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
        "return_type"_a = glsl_type_as_string(signature.to),
        "name"_a        = name,
        "params"_a      = gen_params(signature)
    );
}

struct Params__gen_function_definition {
    FunctionSignature const& signature;
    std::string_view         name;
    std::string_view         body;
};

static auto gen_function_definition(Params__gen_function_definition p)
    -> std::string
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

static auto gen_function__impl(Params__gen_function_definition p)
    -> Function
{
    return {
        .name       = std::string{p.name},
        .definition = gen_function_definition(p),
    };
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
    return "_" + s;                                                             // We need a prefix to make sure `s` does not start with a number.
}

static auto to_string(FunctionSignature signature) -> std::string
{
    return fmt::format("{}_to_{}", cpp_type_as_string(signature.from), cpp_type_as_string(signature.to));
}

static auto base_function_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id
)
    -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR({name}{id})STR"
        ),
        "name"_a = valid_glsl(definition.name()), // NB: We don't have to worry about the uniqueness of that name because we append an ID anyway
        "id"_a   = valid_glsl(to_string(id.underlying_uuid()))
    );
}

static auto desired_function_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id,
    FunctionSignature     signature
)
    -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            R"STR({name}{signature}{id})STR"
        ),
        "name"_a      = valid_glsl(definition.name()), // NB: We don't have to worry about the uniqueness of that name because we append an ID anyway
        "signature"_a = valid_glsl(to_string(signature)),
        "id"_a        = valid_glsl(to_string(id.underlying_uuid()))
    );
}

auto gen_base_function(
    NodeDefinition const& node_definition,
    Cool::NodeId const&   id
)
    -> Function
{
    return gen_function__impl({
        .signature = node_definition.signature(),
        .name      = base_function_name(node_definition, id),
        .body      = node_definition.function_body(),
    });
}

static auto gen_desired_implementation(
    Node const&                                 node,
    FunctionSignature const&                    desired_signature,
    std::string_view                            base_function_name,
    std::string_view                            input_function_name,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition
)
    -> std::optional<FunctionImplementation>
{
    const NodeDefinition* def = get_node_definition(node.definition_name());
    if (!def)
        return std::nullopt;

    return std::visit(
        [&](auto&& current_from, auto&& current_to,
            auto&& desired_from, auto&& desired_to) { return gen_desired_function_implementation(
                                                          current_from, current_to,
                                                          desired_from, desired_to,
                                                          base_function_name, input_function_name
                                                      ); },
        def->signature().from, def->signature().to,
        desired_signature.from, desired_signature.to
    );
}

auto gen_desired_function(
    Cool::NodeId const&                         id,
    FunctionSignature const&                    desired_signature,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Graph const&                                graph
)
    -> std::optional<Function>
{
    const auto node = graph.nodes().get(id);
    if (!node)
        return gen_default_function(desired_signature);

    const auto node_definition = get_node_definition(node->definition_name());
    if (!node_definition)
        return std::nullopt; // TODO(JF) Return unexpected "Definition not found"

    const auto base_function = gen_base_function(*node_definition, id);

    const auto input_function_signature = std::visit(
        [](auto&& A, auto&& B, auto&& C, auto&& D) { return input_function_desired_signature(A, B, C, D); },
        node_definition->signature().from, node_definition->signature().to,
        desired_signature.from, desired_signature.to
    );

    const auto input_function = input_function_signature
                                    ? gen_desired_function(
                                        graph.predecessor_node_id(id),
                                        *input_function_signature,
                                        get_node_definition,
                                        graph
                                    )
                                    : std::make_optional(
                                        Function{
                                            .name       = "",
                                            .definition = "",
                                        }
                                    );
    if (!input_function)
        return input_function;

    const auto name = desired_function_name(*node_definition, id, desired_signature);

    const auto impl = gen_desired_implementation(*node, desired_signature, base_function.name, input_function->name, get_node_definition);
    if (!impl)
        return std::nullopt; // TODO(JF) Return unexpected

    auto function = gen_function__impl({
        .signature = desired_signature,
        .name      = name,
        .body      = impl->function_body,
    });

    function.definition = input_function->definition + "\n\n"
                          + impl->before_function + "\n\n"
                          + base_function.definition + "\n\n"
                          + function.definition;

    return function;
}

auto generate_shader_code(
    Graph const&                                       graph,
    Cool::GetNodeDefinition_Ref<NodeDefinition> const& get_node_definition,
    Cool::NodeId const&                                main_node_id
)
    -> std::string
{
    const auto main_function = gen_desired_function(
        main_node_id,
        Signature::Image,
        get_node_definition,
        graph
    );
    if (!main_function)
        return "";

    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(R"STR(#version 410

uniform float _time;
out vec4      out_Color;

#include "_ROOT_FOLDER_/res/shader-lib/normalized_uv.glsl"

{main_function_definition}

void main()
{{
    vec2 uv = normalized_uv();
    out_Color = vec4({main_function_name}(uv), 1.);
}}

)STR"
        ),
        "main_function_definition"_a = main_function->definition,
        "main_function_name"_a       = main_function->name
    );
}

} // namespace Lab