#include "generate_shader_code.h"
#include <Cool/InputParser/InputParser.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include <Cool/String/String.h>
#include <fmt/compile.h>
#include "Function.h"
#include "FunctionSignature.h"
#include "Node.h"
#include "NodeDefinition.h"
#include "gen_default_function.h"
#include "gen_desired_function_implementation.h"

namespace Lab {

static auto gen_desired_function(
    Cool::NodeId const&                         id,
    FunctionSignature const&                    desired_signature,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Graph const&                                graph,
    Cool::InputProvider_Ref                     input_provider
) -> tl::expected<Function, std::string>;

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
    FunctionSignature const& signature{};
    std::string_view         name{};
    std::string_view         body{};
    std::string_view         before_function{};
};

static auto gen_function_definition(Params__gen_function_definition p)
    -> std::string
{
    using namespace fmt::literals;
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
        "declaration"_a     = gen_function_declaration(p.signature, p.name),
        "body"_a            = p.body
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
) -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            "{name}{signature}{id}"
        ),
        "name"_a      = valid_glsl(definition.name()), // NB: We don't have to worry about the uniqueness of that name because we append an ID anyway
        "signature"_a = valid_glsl(to_string(signature)),
        "id"_a        = valid_glsl(to_string(id.underlying_uuid()))
    );
}

static auto gen_properties(
    std::vector<Cool::AnyInput> const& properties,
    Cool::InputProvider_Ref            input_provider
) -> std::string
{
    using namespace fmt::literals;
    std::stringstream res{};

    for (auto const& prop : properties)
        res << Cool::gen_input_shader_code(
            prop,
            input_provider,
            std::visit([](auto&& prop) { return fmt::format("`{}`", prop.name()); }, prop) // Re-add backticks around the name so the names are generated the same as users have used in their function body. This will allow the replacement that comes next to handle everybody uniformly.
        ) << '\n';

    return res.str();
}

auto valid_property_name(std::string const& name, reg::AnyId const& property_default_variable_id) // We use a unique id per property to make sure they don't clash with anything. For example if the node was called Zoom and its property was also called Zoom, both the function and the uniform variable would get the same name.
    -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            "{name}{id}"
        ),
        "name"_a = valid_glsl(name),
        "id"_a   = valid_glsl(to_string(property_default_variable_id.underlying_uuid()))
    );
}

static auto list_all_property_and_input_names(std::vector<Cool::AnyInput> const& properties, std::vector<NodeInputDefinition> const& inputs)
    -> std::string
{
    std::stringstream res{};

    for (auto const& prop : properties)
        res << fmt::format("  - `{}`", std::visit([](auto&& prop) { return prop.name(); }, prop)) << '\n';
    for (auto const& input : inputs)
        res << fmt::format("  - `{}`", input.name()) << '\n';

    return res.str();
}

static auto replace_property_names(
    std::string                        code,
    std::vector<Cool::AnyInput> const& properties
) -> std::string
{
    for (auto const& prop : properties)
    {
        std::visit([&](auto&& prop) {
            Cool::String::replace_all(code, fmt::format("`{}`", prop.name()), valid_property_name(prop.name(), prop._default_variable_id));
        },
                   prop);
    }

    return code;
}

static auto replace_input_names(
    std::string                                         code,
    std::unordered_map<std::string, std::string> const& real_names
) -> std::string
{
    for (auto const& [old_name, new_name] : real_names)
        Cool::String::replace_all(code, fmt::format("`{}`", old_name), new_name);

    return code;
}

static auto check_there_are_no_backticks_left(std::string const& code, std::string const& input_names_list)
    -> std::optional<std::string>
{
    auto const pos = code.find('`');
    if (pos == std::string::npos)
        return std::nullopt;

    auto const pos2 = code.find('`', pos + 1);
    if (pos2 == std::string::npos)
        return "A backtick (`) has no matching closing backtick. Did you make a typo?";

    return fmt::format(
        "\"{}\" is not a valid input name. Here are all the inputs you defined:\n{}",
        Cool::String::substring(code, pos, pos2 + 1),
        input_names_list
    );
}

struct GeneratedInputs {
    std::string                                  code;
    std::unordered_map<std::string, std::string> real_names;
};

static auto gen_inputs(
    Node const&                                 node,
    NodeDefinition const&                       node_definition,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Graph const&                                graph,
    Cool::InputProvider_Ref                     input_provider
)
    -> tl::expected<GeneratedInputs, std::string>
{
    GeneratedInputs res;

    for (auto const& input : node_definition.inputs())
    {
        auto const function = gen_desired_function(
            graph.input_node_id(node.input_pins()[1].id()), // TODO(JF) Get pin index properly
            input.signature(),
            get_node_definition,
            graph,
            input_provider
        );
        if (!function)
            return tl::make_unexpected(function.error());

        res.code += function->definition;
        res.real_names[input.name()] = function->name;
    }

    return res;
}

static auto gen_base_function(
    Node const&                                 node,
    NodeDefinition const&                       node_definition,
    Cool::NodeId const&                         id,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Graph const&                                graph,
    Cool::InputProvider_Ref                     input_provider
) -> tl::expected<Function, std::string>
{
    auto const inputs = gen_inputs(node, node_definition, get_node_definition, graph, input_provider);
    if (!inputs)
        return tl::make_unexpected(inputs.error());

    auto const func = gen_function__impl({
        .signature       = node_definition.signature(),
        .name            = base_function_name(node_definition, id),
        .body            = node_definition.function_body(),
        .before_function = gen_properties(node.properties(), input_provider) + "\n\n"
                           + inputs->code,
    });

    // { // Add a "namespace" to all the names that this function has defined globally (like its properties) so that names don't clash with another instance of the same node.
    auto definition = replace_property_names(func.definition, node.properties());
    definition      = replace_input_names(definition, inputs->real_names);

    {
        auto const error = check_there_are_no_backticks_left(definition, list_all_property_and_input_names(node.properties(), node_definition.inputs()));
        if (error)
            return tl::make_unexpected(*error);
    }
    // }

    return Function{
        .name       = func.name,
        .definition = definition,
    };
}

static auto gen_desired_implementation(
    NodeDefinition const&    node_definition,
    FunctionSignature const& desired_signature,
    std::string_view         base_function_name,
    std::string_view         input_function_name
)
    -> FunctionImplementation
{
    return std::visit(
        [&](auto&& current_from, auto&& current_to,
            auto&& desired_from, auto&& desired_to) { return gen_desired_function_implementation(
                                                          current_from, current_to,
                                                          desired_from, desired_to,
                                                          base_function_name, input_function_name
                                                      ); },
        node_definition.signature().from, node_definition.signature().to,
        desired_signature.from, desired_signature.to
    );
}

static auto gen_desired_function(
    Cool::NodeId const&                         id,
    FunctionSignature const&                    desired_signature,
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
    Graph const&                                graph,
    Cool::InputProvider_Ref                     input_provider
)
    -> tl::expected<Function, std::string>
{
    const auto node = graph.nodes().get(id);
    if (!node)
        return gen_default_function(desired_signature);

    const auto node_definition = get_node_definition(node->definition_name());
    if (!node_definition)
        return tl::make_unexpected(fmt::format(
            "Node definition \"{}\" was not found. Are you missing a file in your nodes folder?",
            node->definition_name()
        ));

    const auto base_function = gen_base_function(
        *node, *node_definition, id,
        get_node_definition, graph, input_provider
    );
    if (!base_function)
        return tl::make_unexpected(fmt::format(
            "Code for node \"{}\" is invalid:\n{}",
            node_definition->name(), base_function.error()
        ));

    const auto input_function_signature = std::visit(
        [](auto&& A, auto&& B, auto&& C, auto&& D) { return input_function_desired_signature(A, B, C, D); },
        node_definition->signature().from, node_definition->signature().to,
        desired_signature.from, desired_signature.to
    );

    const auto input_function = input_function_signature
                                    ? gen_desired_function(
                                        graph.input_node_id(node->input_pins()[0].id()), // TODO(JF) Cleaner way to get the main pin id
                                        *input_function_signature,
                                        get_node_definition,
                                        graph,
                                        input_provider
                                    )
                                    : tl::expected<Function, std::string>{
                                        Function{
                                            .name       = "",
                                            .definition = "",
                                        }};
    if (!input_function)
        return input_function;

    const auto name = desired_function_name(*node_definition, id, desired_signature);

    const auto impl = gen_desired_implementation(*node_definition, desired_signature, base_function->name, input_function->name);

    auto function = gen_function__impl({
        .signature = desired_signature,
        .name      = name,
        .body      = impl.function_body,
    });

    function.definition = input_function->definition + "\n\n"
                          + impl.before_function + "\n\n"
                          + base_function->definition + "\n\n"
                          + function.definition;

    return function;
}

auto generate_shader_code(
    Graph const&                                       graph,
    Cool::GetNodeDefinition_Ref<NodeDefinition> const& get_node_definition,
    Cool::NodeId const&                                main_node_id,
    Cool::InputProvider_Ref                            input_provider
)
    -> tl::expected<std::string, std::string>
{
    const auto main_function = gen_desired_function(
        main_node_id,
        Signature::Image,
        get_node_definition,
        graph,
        input_provider
    );
    if (!main_function)
        return tl::make_unexpected(fmt::format("Failed to generate shader code:\n{}", main_function.error()));

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