#include "CodeGen.h"
#include <Cool/InputParser/InputParser.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include <Cool/String/String.h>
#include "CodeGenContext.h"
#include "CodeGen_default_function.h"
#include "CodeGen_desired_function_implementation.h"
#include "Function.h"
#include "FunctionSignature.h"
#include "Node.h"
#include "NodeDefinition.h"
#include "input_to_primitive_type.h"

namespace Lab {

static auto gen_params(FunctionSignature const& signature)
    -> std::string
{
    using namespace fmt::literals;

    std::string res{};
    for (size_t i = 0; i < signature.arity; ++i)
    {
        res += fmt::format(
            FMT_COMPILE(
                "{type} in{index}"
            ),
            "type"_a  = glsl_type_as_string(signature.from),
            "index"_a = i + 1
        );
        if (i != signature.arity - 1)
            res += ", ";
    }

    return res;
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
    std::string_view         before_function{};
    std::string_view         body{};
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

auto make_valid_output_index_name(Cool::OutputPin const& pin)
    -> std::string
{
    using namespace fmt::literals;
    return fmt::format(
        FMT_COMPILE(
            "{name}{id}"
        ),
        "name"_a = valid_glsl(pin.name()),
        "id"_a   = valid_glsl(to_string(pin.id().underlying_uuid()))
    );
}

struct Properties {
    std::string              code;
    std::vector<std::string> real_names;
};

static auto gen_properties(
    Node const&     node,
    CodeGenContext& context
) -> tl::expected<Properties, std::string>
{
    using namespace fmt::literals;
    Properties res{};

    size_t property_index{0};
    for (auto const& prop : node.properties())
    {
        auto const input_pin     = node.pin_of_property(property_index);
        auto       output_pin    = Cool::OutputPin{};
        auto const input_node_id = context.graph().input_node_id(input_pin.id(), &output_pin);
        auto const node          = context.graph().nodes().get(input_node_id);
        if (node)
        {
            if (node->main_output_pin() == output_pin)
            {
                auto const property_type = input_to_primitive_type(prop);
                if (!property_type)
                    return tl::make_unexpected("Can't create property with that type"); // TODO(JF) Improve error message

                auto const input_func_name = gen_desired_function(
                    {.from = PrimitiveType::UV, .to = *property_type},
                    input_node_id,
                    context
                );
                if (!input_func_name)
                    return tl::make_unexpected(input_func_name.error());

                res.real_names.push_back(fmt::format("{}(normalized_uv())", *input_func_name)); // Input name will be replaced with a call to the corresponding function
            }
            else // We are plugged to an output index
            {
                res.real_names.push_back(make_valid_output_index_name(output_pin));
            }
        }
        else
        {
            res.code += Cool::gen_input_shader_code(
                            prop,
                            context.input_provider(),
                            std::visit([](auto&& prop) { return fmt::format("`{}`", prop.name()); }, prop) // Re-add backticks around the name so the names are generated the same as users have used in their function body. This will allow the replacement that comes next to handle everybody uniformly.
                        )
                        + '\n';

            res.real_names.push_back(std::visit(
                [](auto&& prop) { return valid_property_name(prop.name(), prop._default_variable_id); }, prop
            ));
        }

        property_index++;
    }

    return res;
}

static auto list_all_property_and_input_and_output_names(
    std::vector<Cool::AnyInput> const&      properties,
    std::vector<NodeInputDefinition> const& inputs,
    std::vector<std::string> const&         output_indices_names
)
    -> std::string
{
    std::string res{};

    for (auto const& prop : properties)
        res += fmt::format("  - `{}`\n", std::visit([](auto&& prop) { return prop.name(); }, prop));
    for (auto const& input : inputs)
        res += fmt::format("  - `{}`\n", input.name());
    for (auto const& name : output_indices_names)
        res += fmt::format("  - `{}`\n", name);

    return res;
}

static auto replace_property_names(
    std::string                        code,
    std::vector<Cool::AnyInput> const& properties,
    std::vector<std::string> const&    real_name
) -> std::string
{
    size_t i{0};
    for (auto const& prop : properties)
    {
        std::visit([&](auto&& prop) {
            Cool::String::replace_all(code, fmt::format("`{}`", prop.name()), real_name[i]);
        },
                   prop);
        i++;
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

static auto replace_output_indices_names(
    std::string code,
    Node const& node
) -> std::string
{
    for (size_t i = 1; i < node.output_pins().size(); ++i)
    {
        Cool::String::replace_all(
            code,
            fmt::format("`{}`", node.output_pins()[i].name()),
            make_valid_output_index_name(node.output_pins()[i])
        );
    }

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
    std::unordered_map<std::string, std::string> real_names;
};

static auto gen_inputs(
    Node const&           node,
    NodeDefinition const& node_definition,
    CodeGenContext&       context
) -> tl::expected<GeneratedInputs, std::string>
{
    GeneratedInputs res;

    for (size_t input_idx = 0; input_idx < node_definition.inputs().size(); ++input_idx)
    {
        auto const& input         = node_definition.inputs()[input_idx];
        auto        output_pin    = Cool::OutputPin{};
        auto const  input_node_id = context.graph().input_node_id(node.pin_of_input(input_idx).id(), &output_pin);
        auto const  input_node    = context.graph().nodes().get(input_node_id);
        if (!input_node || output_pin == input_node->main_output_pin()) // If we are plugged to the main output of a node (or to nothing, in which case we will generate a default function), then generate the corresponding function
        {
            auto const func_name = gen_desired_function(
                input.signature(),
                input_node_id,
                context
            );
            if (!func_name)
                return tl::make_unexpected(func_name.error());

            res.real_names[input.name()] = *func_name;
        }
        else // We are plugged to an output index
        {
            res.real_names[input.name()] = make_valid_output_index_name(output_pin);
        }
    }

    return res;
}

static auto gen_base_function(
    Node const&           node,
    NodeDefinition const& node_definition,
    Cool::NodeId const&   id,
    CodeGenContext&       context
) -> ExpectedFunctionName
{
    auto const inputs = gen_inputs(node, node_definition, context);
    if (!inputs)
        return tl::make_unexpected(inputs.error());

    auto const properties_code = gen_properties(node, context);
    if (!properties_code)
        return tl::make_unexpected(properties_code.error());

    auto const func_name = base_function_name(node_definition, id);

    auto func_implementation = gen_function_definition({
        .signature       = node_definition.signature(),
        .name            = func_name,
        .before_function = properties_code->code,
        .body            = node_definition.function_body(),
    });

    // Add a "namespace" to all the names that this function has defined globally (like its properties) so that names don't clash with another instance of the same node.
    func_implementation = replace_property_names(func_implementation, node.properties(), properties_code->real_names);
    func_implementation = replace_input_names(func_implementation, inputs->real_names);
    func_implementation = replace_output_indices_names(func_implementation, node);

    {
        auto const error = check_there_are_no_backticks_left(func_implementation, list_all_property_and_input_and_output_names(node.properties(), node_definition.inputs(), node_definition.output_indices()));
        if (error)
            return tl::make_unexpected(*error);
    }

    context.push_function({.name = func_name, .implementation = func_implementation});

    return func_name;
}

auto gen_desired_function(
    FunctionSignature   desired_signature,
    Cool::NodeId const& id,
    CodeGenContext&     context
) -> ExpectedFunctionName
{
    auto const node = context.graph().nodes().get(id);
    if (!node) // Use a default function if no node is currently plugged in
        return gen_default_function(desired_signature, context);

    auto const node_definition = context.get_node_definition(node->definition_name());
    if (!node_definition)
        return tl::make_unexpected(fmt::format(
            "Node definition \"{}\" was not found. Are you missing a file in your nodes folder?",
            node->definition_name()
        ));

    auto const base_function_name = gen_base_function(*node, *node_definition, id, context);
    if (!base_function_name)
        return tl::make_unexpected(fmt::format(
            "Failed to generate code for node \"{}\":\n{}",
            node_definition->name(), base_function_name.error()
        ));

    auto const func_body = gen_desired_function_implementation(
        node_definition->signature(),
        desired_signature,
        *base_function_name,
        *node,
        context
    );
    if (!func_body)
        return tl::make_unexpected(fmt::format(
            "Failed to generate conversion code for node \"{}\":\n{}",
            node_definition->name(), func_body.error()
        ));

    auto const func_name       = desired_function_name(*node_definition, id, desired_signature);
    auto const func_definition = gen_function_definition({
        .signature       = desired_signature,
        .name            = func_name,
        .before_function = "",
        .body            = *func_body,
    });

    context.push_function({.name = func_name, .implementation = func_definition});

    return func_name;
}

} // namespace Lab