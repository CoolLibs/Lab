#include "CodeGen.h"
#include <Cool/Expected/RETURN_IF_UNEXPECTED.h>
#include <Cool/InputParser/InputParser.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include <Cool/String/String.h>
#include <Nodes/PrimitiveType.h>
#include <fmt/core.h>
#include <optional>
#include <string>
#include "CodeGenContext.h"
#include "CodeGen_default_function.h"
#include "CodeGen_desired_function_implementation.h"
#include "Cool/Nodes/Pin.h"
#include "Function.h"
#include "FunctionSignature.h"
#include "Node.h"
#include "NodeDefinition.h"
#include "helpers.h"
#include "input_to_primitive_type.h"

namespace Lab {

static auto gen_params(CompleteFunctionSignature const& signature)
    -> std::string
{
    using fmt::literals::operator""_a;

    std::string res{};
    for (size_t i = 0; i < signature.parameters.size(); ++i)
    {
        res += fmt::format(
            FMT_COMPILE(
                "{type} {name}"
            ),
            "type"_a = glsl_type_as_string(signature.parameters[i].type),
            "name"_a = signature.parameters[i].name
        );
        if (i != signature.parameters.size() - 1)
            res += ", ";
    }

    return res;
}

static auto gen_function_declaration(
    CompleteFunctionSignature const& signature,
    std::string_view                 name
) -> std::string
{
    using fmt::literals::operator""_a;
    return fmt::format(
        FMT_COMPILE(
            R"STR({return_type} {name}/*coollabdef*/({params}))STR"
        ),
        "return_type"_a = glsl_type_as_string(signature.output_type),
        "name"_a        = name,
        "params"_a      = gen_params(signature)
    );
}

struct Params_gen_function_definition {
    CompleteFunctionSignature const& signature{};
    std::string_view                 name{};
    std::string_view                 before_function{};
    std::string_view                 body{};
};

static auto gen_function_definition(Params_gen_function_definition p)
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
        "declaration"_a     = gen_function_declaration(p.signature, p.name),
        "body"_a            = p.body
    )};
}

static auto base_function_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id
)
    -> std::string
{
    using fmt::literals::operator""_a;
    return valid_glsl(
        fmt::format(
            FMT_COMPILE(
                R"STR({name}{id})STR"
            ),
            "name"_a = definition.name(), // NB: We don't have to worry about the uniqueness of that name because we append an ID anyway
            "id"_a   = to_string(id.underlying_uuid())
        )
    );
}

static auto desired_function_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id,
    FunctionSignature     signature
) -> std::string
{
    using fmt::literals::operator""_a;
    return valid_glsl(
        fmt::format(
            FMT_COMPILE(
                "{name}{signature}{id}"
            ),
            "name"_a      = definition.name(), // NB: We don't have to worry about the uniqueness of that name because we append an ID anyway
            "signature"_a = to_string(signature),
            "id"_a        = to_string(id.underlying_uuid())
        )
    );
}

auto module_texture_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id
)
    -> std::string
{
    using fmt::literals::operator""_a;
    return valid_glsl(
        fmt::format(
            FMT_COMPILE(
                R"STR(texture_{name}{id})STR"
            ),
            "name"_a = definition.name(),
            "id"_a   = to_string(id.underlying_uuid())
        )
    );
}

auto valid_property_name(std::string const& name, reg::AnyId const& property_default_variable_id) // We use a unique id per property to make sure they don't clash with anything. For example if the node was called Zoom and its property was also called Zoom, both the function and the uniform variable would get the same name.
    -> std::string
{
    using fmt::literals::operator""_a;
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
    using fmt::literals::operator""_a;
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

static auto gen_value_inputs(
    Node const&                   node,
    CodeGenContext&               context,
    NodeDefinitionCallback const& node_definition_callback
) -> tl::expected<Properties, std::string>
{
    using fmt::literals::operator""_a;
    Properties           res{};

    size_t property_index{0};
    for (auto const& prop : node.value_inputs())
    {
        auto const input_pin     = node.pin_of_value_input(property_index); // NOLINT(performance-unnecessary-copy-initialization)
        auto       output_pin    = Cool::OutputPin{};
        auto const input_node_id = context.graph().find_node_connected_to_input_pin(input_pin.id(), &output_pin);
        auto const maybe_node    = context.graph().try_get_node<Node>(input_node_id);
        if (maybe_node)
        {
            if (maybe_node->main_output_pin() == output_pin)
            {
                auto const property_type = input_to_primitive_type(prop);
                if (!property_type)
                    return tl::make_unexpected("Can't create property with that type"); // TODO(JF) Improve error message

                auto const input_func_name = gen_desired_function(
                    {.from = PrimitiveType::Void, .to = *property_type, .arity = 0},
                    *maybe_node,
                    input_node_id,
                    context,
                    node_definition_callback
                );
                RETURN_IF_UNEXPECTED(input_func_name);

                res.real_names.push_back(fmt::format("{}()", *input_func_name)); // Input name will be replaced with a call to the corresponding function
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
                            std::visit([](auto&& prop) { return fmt::format("'{}'", prop.name()); }, prop) // Re-add single quotes around the name so the names are generated the same as users have used in their function body. This will allow the replacement that comes next to handle everybody uniformly.
                        )
                        + '\n';

            res.real_names.push_back(std::visit(
                [](auto&& prop) { return valid_property_name(prop.name(), prop._default_variable_id.raw()); }, prop
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
        res += fmt::format("  - '{}'\n", std::visit([](auto&& prop) { return prop.name(); }, prop));
    for (auto const& input : inputs)
        res += fmt::format("  - '{}'\n", input.name());
    for (auto const& name : output_indices_names)
        res += fmt::format("  - '{}'\n", name);

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
            Cool::String::replace_all(code, fmt::format("'{}'", prop.name()), real_name[i]);
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
        Cool::String::replace_all(code, fmt::format("'{}'", old_name), new_name);

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
            fmt::format("'{}'", node.output_pins()[i].name()),
            make_valid_output_index_name(node.output_pins()[i])
        );
    }

    return code;
}

static auto check_there_are_no_single_quotes_left(std::string const& code, std::string const& input_names_list)
    -> std::optional<std::string>
{
    auto const pos = code.find('\'');
    if (pos == std::string::npos)
        return std::nullopt;

    auto const pos2 = code.find('\'', pos + 1);
    if (pos2 == std::string::npos)
        return "A single quote (') has no matching closing single quote. Did you make a typo?";

    return fmt::format(
        "\"{}\" is not an input or output that you declared. Here are all the names you declared:\n{}",
        Cool::String::substring(code, pos, pos2 + 1),
        input_names_list
    );
}

static auto replace_helper_functions(std::string code, std::vector<FunctionPieces> const& old_functions, std::vector<std::string> const& new_names)
    -> std::string
{
    assert(old_functions.size() == new_names.size());

    for (size_t i = 0; i < new_names.size(); ++i)
        code = Cool::String::replace_all_words(code, old_functions[i].name, new_names[i]);

    return code;
}

struct GeneratedInputs {
    std::unordered_map<std::string, std::string> real_names;
};

static auto gen_function_inputs(
    Node const&                   node,
    NodeDefinition const&         node_definition,
    CodeGenContext&               context,
    NodeDefinitionCallback const& node_definition_callback
) -> tl::expected<GeneratedInputs, std::string>
{
    GeneratedInputs res;

    for (size_t fn_input_idx = 0; fn_input_idx < node_definition.function_inputs().size(); ++fn_input_idx)
    {
        auto const& fn_input      = node_definition.function_inputs()[fn_input_idx];
        auto        output_pin    = Cool::OutputPin{};
        auto const  input_node_id = context.graph().find_node_connected_to_input_pin(node.pin_of_function_input(fn_input_idx).id(), &output_pin);
        auto const  input_node    = context.graph().try_get_node<Node>(input_node_id);
        if (!input_node || output_pin == input_node->main_output_pin()) // If we are plugged to the main output of a node (or to nothing, in which case we will generate a default function), then generate the corresponding function
        {
            auto const func_name = gen_desired_function(
                fn_input.signature(),
                input_node,
                input_node_id,
                context,
                node_definition_callback
            );
            RETURN_IF_UNEXPECTED(func_name);

            res.real_names[fn_input.name()] = *func_name;
        }
        else // We are plugged to an output index
        {
            res.real_names[fn_input.name()] = make_valid_output_index_name(output_pin);
        }
    }

    return res;
}

struct GeneratedHelperFunctions {
    std::string              code;
    std::vector<std::string> new_names;
};

static auto gen_helper_functions(std::vector<FunctionPieces> const& helper_functions, Cool::NodeId const& id)
    -> GeneratedHelperFunctions
{
    auto res = GeneratedHelperFunctions{};

    for (auto const& func : helper_functions)
    {
        auto const name = valid_glsl(fmt::format("{}{}", func.name, to_string(id.underlying_uuid())));
        res.new_names.push_back(name);

        res.code += gen_function_definition({
            .signature = func.signature,
            .name      = name,
            .body      = func.body,
        });
        res.code += '\n';
    }

    return res;
}

static auto gen_includes(NodeDefinition const& node_definition)
    -> std::string
{
    auto res = std::string{};
    for (auto const& path : node_definition.included_files())
        res += fmt::format("#include \"{}\"\n", path.string());
    return res;
}

static auto gen_base_function(
    Node const&                   node,
    NodeDefinition const&         node_definition,
    Cool::NodeId const&           id,
    CodeGenContext&               context,
    NodeDefinitionCallback const& node_definition_callback
) -> ExpectedFunctionName
{
    auto const function_inputs = gen_function_inputs(node, node_definition, context, node_definition_callback);
    RETURN_IF_UNEXPECTED(function_inputs);

    auto const value_inputs_code = gen_value_inputs(node, context, node_definition_callback);
    RETURN_IF_UNEXPECTED(value_inputs_code);

    auto const func_name = base_function_name(node_definition, id);

    auto const helper_functions = gen_helper_functions(node_definition.helper_functions(), id);

    // HACK to make sure we are aware that these functions have been generated, used when adding a parameter to all the functions during `inject_context_argument_in_all_functions()`.
    // We don't care about adding them through push_function() because their names will be unique anyways.
    // And we need them to be defined after properties_code->code
    for (auto const& helper_func_name : helper_functions.new_names)
        context.push_function(Function{.name = helper_func_name, .definition = ""});

    auto func_implementation = gen_function_definition({
        .signature       = make_complete_function_signature(MainFunctionSignature{
                  .signature       = node_definition.signature(),
                  .parameter_names = node_definition.parameter_names(),
        }),
        .name            = func_name,
        .before_function = gen_includes(node_definition) + '\n'
                           + value_inputs_code->code + '\n'
                           + helper_functions.code,
        .body = node_definition.function_body(),
    });

    // Add a "namespace" to all the names that this function has defined globally (like its value inputs) so that names don't clash with another instance of the same node.
    func_implementation = replace_property_names(func_implementation, node.value_inputs(), value_inputs_code->real_names);
    func_implementation = replace_input_names(func_implementation, function_inputs->real_names);
    func_implementation = replace_output_indices_names(func_implementation, node);
    func_implementation = replace_helper_functions(func_implementation, node_definition.helper_functions(), helper_functions.new_names);

    {
        auto const error = check_there_are_no_single_quotes_left(func_implementation, list_all_property_and_input_and_output_names(node.value_inputs(), node_definition.function_inputs(), node_definition.output_indices()));
        if (error)
            return tl::make_unexpected(*error);
    }

    context.push_function({.name = func_name, .definition = func_implementation});

    return func_name;
}

static auto make_default_parameter_names(size_t arity)
    -> std::vector<std::string>
{
    auto res = std::vector<std::string>{};

    for (size_t i = 0; i < arity; ++i)
        res.push_back(fmt::format("in{}", i + 1));

    return res;
}

static auto gen_output_function(Cool::OutputPin const& pin, CodeGenContext& context)
    -> ExpectedFunctionName
{
    auto const output_name = make_valid_output_index_name(pin);
    auto const func_name   = fmt::format("get{}", output_name);

    return context.push_function({
        .name       = func_name,
        .definition = fmt::format(
            R"STR(
vec2 {}/*coollabdef*/()
{{
    return vec2({}, 1.); // Convert float to float_and_alpha
}}
)STR",
            func_name, output_name
        ),
    });
}

auto gen_desired_function(
    FunctionSignature             desired_signature,
    Cool::InputPin const&         pin,
    CodeGenContext&               context,
    NodeDefinitionCallback const& node_definition_callback,
    bool                          fallback_to_a_default_function
) -> ExpectedFunctionName
{
    Cool::OutputPin output_pin;
    auto const      node_id = context.graph().find_node_connected_to_input_pin(pin.id(), &output_pin);
    auto const      node    = context.graph().try_get_node<Node>(node_id);

    if (node && output_pin != node->main_output_pin())
    // We are plugged to an ouput index, use that to generate a constant function.
    {
        return gen_output_function(output_pin, context);
    }

    return gen_desired_function(
        desired_signature,
        node,
        node_id,
        context,
        node_definition_callback,
        fallback_to_a_default_function
    );
}

auto gen_desired_function(
    FunctionSignature             desired_signature,
    Cool::NodeId const&           id,
    CodeGenContext&               context,
    NodeDefinitionCallback const& node_definition_callback,
    bool                          fallback_to_a_default_function
) -> ExpectedFunctionName
{
    auto const maybe_node = context.graph().try_get_node<Node>(id);

    return gen_desired_function(
        desired_signature,
        maybe_node,
        id,
        context,
        node_definition_callback,
        fallback_to_a_default_function
    );
}

auto gen_desired_function(
    FunctionSignature             desired_signature,
    Node const*                   maybe_node,
    Cool::NodeId const&           id,
    CodeGenContext&               context,
    NodeDefinitionCallback const& node_definition_callback,
    bool                          fallback_to_a_default_function
) -> ExpectedFunctionName
{
    if (!maybe_node)
    {
        if (!fallback_to_a_default_function)
            return "";

        return gen_default_function(desired_signature, context);
    }

    return gen_desired_function(
        desired_signature,
        *maybe_node,
        id,
        context,
        node_definition_callback
    );
}

auto gen_desired_function(
    FunctionSignature                  desired_signature,
    std::reference_wrapper<Node const> node,
    Cool::NodeId const&                id,
    CodeGenContext&                    context,
    NodeDefinitionCallback const&      node_definition_callback
) -> ExpectedFunctionName
{
    auto node_definition = context.get_node_definition(node.get().id_names()); // NOLINT(readability-qualified-auto)
    if (!node_definition)
        return tl::make_unexpected(fmt::format(
            "Node definition \"{}\" was not found. Are you missing a file in your nodes folder?",
            node.get().definition_name()
        ));

    std::optional<std::string> const maybe_texture_name = node_definition_callback(id, *node_definition);

    Node                                      new_node;
    tl::expected<NodeDefinition, std::string> new_node_definition;
    if (maybe_texture_name.has_value())
    {
        using fmt::literals::operator""_a;
        auto const           main_function_signature = MainFunctionSignature{
            FunctionSignature{
                          .from  = PrimitiveType::UV,
                          .to    = PrimitiveType::sRGB_StraightA,
                          .arity = 1,
            },
            std::vector<std::string>{"uv"}};
        auto const main_function_pieces = MainFunctionPieces{
            .name      = "read_particle_texture",
            .signature = main_function_signature,
            .body      = fmt::format(R"glsl(
uv = unnormalize_uv(to_view_space(uv));
return texture({texture_name}, uv);
)glsl",
                                     "texture_name"_a = *maybe_texture_name),
        };
        new_node_definition = NodeDefinition::make(
            NodeDefinition_Data{
                .main_function    = main_function_pieces,
                .helper_functions = {},
                .included_files   = {},
                .input_functions  = {},
                .input_values     = {},
                .output_indices   = {},
            },
            {}
        );

        new_node = Node(Cool::NodeDefinitionIdentifier{.definition_name = "get_module_texture", .category_name = "get_module_texture"}, 0, 0);
        node     = new_node;

        // We control the node definition callback, so we know that the node definition we are getting is valid.
        assert(new_node_definition.has_value());
        node_definition = &new_node_definition.value();
    }

    auto const base_function_name = gen_base_function(node, *node_definition, id, context, node_definition_callback);
    if (!base_function_name)
        return tl::make_unexpected(fmt::format(
            "Failed to generate code for node \"{}\":\n{}",
            node_definition->name(), base_function_name.error()
        ));

    auto const func_body = gen_desired_function_implementation(
        node_definition->signature(),
        desired_signature,
        *base_function_name,
        node,
        id,
        context,
        node_definition_callback
    );
    if (!func_body)
        return tl::make_unexpected(fmt::format(
            "Failed to generate conversion code for node \"{}\":\n{}",
            node_definition->name(), func_body.error()
        ));

    auto const func_name       = desired_function_name(*node_definition, id, desired_signature);
    auto const func_definition = gen_function_definition({
        .signature       = make_complete_function_signature(MainFunctionSignature{
                  .signature       = desired_signature,
                  .parameter_names = make_default_parameter_names(desired_signature.arity),
        }),
        .name            = func_name,
        .before_function = "",
        .body            = *func_body,
    });

    context.push_function({.name = func_name, .definition = func_definition});

    return func_name;
}

} // namespace Lab
