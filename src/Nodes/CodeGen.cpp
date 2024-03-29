#include "CodeGen.h"
#include <Cool/Expected/RETURN_IF_UNEXPECTED.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodeId.h>
#include <Cool/String/String.h>
#include <Cool/Variables/gen_input_shader_code.h>
#include <Nodes/PrimitiveType.h>
#include <fmt/core.h>
#include <optional>
#include <string>
#include "CodeGenContext.h"
#include "CodeGen_default_function.h"
#include "CodeGen_desired_function_implementation.h"
#include "Cool/Expected/RETURN_IF_ERROR.h"
#include "Cool/Nodes/Pin.h"
#include "Function.h"
#include "FunctionSignature.h"
#include "Node.h"
#include "NodeDefinition.h"
#include "gen_function_definition.h"
#include "valid_glsl.h"
#include "valid_input_name.h"
#include "variable_to_primitive_type.h"

namespace Lab {

static auto base_function_name(
    NodeDefinition const& definition,
    Cool::NodeId const&   id
)
    -> std::string
{
    using fmt::literals::operator""_a;
    return valid_glsl(fmt::format(
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
    return valid_glsl(fmt::format(
        FMT_COMPILE(
            "{name}{signature}{id}"
        ),
        "name"_a      = definition.name(), // NB: We don't have to worry about the uniqueness of that name because we append an ID anyway
        "signature"_a = to_string(signature),
        "id"_a        = to_string(id.underlying_uuid())
    )
    );
}

auto make_valid_output_index_name(Cool::OutputPin const& pin) -> std::string
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

struct ValueInputs {
    std::string              code;
    std::vector<std::string> real_names;
};

static auto gen_value_inputs(
    Node const&                node,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module
) -> tl::expected<ValueInputs, std::string>
{
    using fmt::literals::operator""_a;
    ValueInputs res{};

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
                auto const property_type = variable_to_primitive_type(prop);
                if (!property_type)
                    return tl::make_unexpected("Can't create an INPUT with that type"); // TODO(JF) Improve error message

                auto const input_func_name = gen_desired_function(
                    {.from = PrimitiveType::Void, .to = *property_type, .arity = 0},
                    *maybe_node,
                    input_node_id,
                    context,
                    maybe_generate_module
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
            res.code += std::visit([](auto&& prop) { return Cool::gen_input_shader_code(
                                                         prop.value(),
                                                         fmt::format("'{}'", prop.name()) // Re-add single quotes around the name so the names are generated the same as users have used in their function body. This will allow the replacement that comes next to handle everybody uniformly.
                                                     ); }, prop)
                        + '\n';

            res.real_names.push_back(valid_input_name(prop));
        }

        property_index++;
    }

    return res;
}

static auto list_all_input_and_output_names(
    std::vector<Cool::AnySharedVariable> const& value_inputs,
    std::vector<NodeInputDefinition> const&     function_inputs,
    std::vector<std::string> const&             output_indices_names
)
    -> std::string
{
    std::string res{};

    for (auto const& value_input : value_inputs)
        res += fmt::format("  - '{}'\n", std::visit([](auto&& value_input) { return value_input.name(); }, value_input));
    for (auto const& function_input : function_inputs)
        res += fmt::format("  - '{}'\n", function_input.name());
    for (auto const& name : output_indices_names)
        res += fmt::format("  - '{}'\n", name);

    return res;
}

static void replace_value_inputs_names(
    std::string&                                code,
    std::vector<Cool::AnySharedVariable> const& value_inputs,
    std::vector<std::string> const&             real_name
)
{
    size_t i{0};
    for (auto const& value_input : value_inputs)
    {
        std::visit([&](auto&& value_input) {
            Cool::String::replace_all_inplace(code, fmt::format("'{}'", value_input.name()), real_name[i]);
        },
                   value_input);
        i++;
    }
}

static void replace_function_inputs_names(std::string& code, std::unordered_map<std::string, std::string> const& real_names)
{
    for (auto const& [old_name, new_name] : real_names)
        Cool::String::replace_all_inplace(code, fmt::format("'{}'", old_name), new_name);
}

static void replace_output_indices_names(std::string& code, Node const& node)
{
    for (size_t i = 1; i < node.output_pins().size(); ++i)
    {
        Cool::String::replace_all_inplace(
            code,
            fmt::format("'{}'", node.output_pins()[i].name()),
            make_valid_output_index_name(node.output_pins()[i])
        );
    }
}

static void replace_names_in_global_scope(std::string& code, std::vector<std::string> const& names_in_global_scope, std::string const& id)
{
    for (auto const& name : names_in_global_scope)
        Cool::String::replace_all_words_inplace(code, name, valid_glsl(fmt::format("{}{}", name, id)));
}

struct GeneratedInputs {
    std::unordered_map<std::string, std::string> real_names;
};

static auto gen_function_inputs(
    Node const&                node,
    NodeDefinition const&      node_definition,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module
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
                maybe_generate_module
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

static auto make_arguments_list(size_t arity, PrimitiveType type)
    -> std::string
{
    auto res = std::string{};

    for (size_t i = 0; i < arity; ++i)
    {
        res += fmt::format("{} in{}", glsl_type_as_string(type), i);
        if (i != arity - 1)
            res += ", ";
    }

    return res;
}

static auto signature_as_string(FunctionSignature signature, std::string const& arguments_list) -> FunctionSignatureAsString
{
    return FunctionSignatureAsString{
        .return_type    = glsl_type_as_string(signature.to),
        .name           = "", // Isn't used anyways
        .arguments_list = arguments_list,
    };
}
static auto signature_as_string(FunctionSignature signature) -> FunctionSignatureAsString
{
    return signature_as_string(signature, make_arguments_list(signature.arity, signature.from));
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

static auto gen_base_function(
    Node const&                node,
    NodeDefinition const&      node_definition,
    Cool::NodeId const&        id,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module
) -> ExpectedFunctionName
{
    auto const function_inputs = gen_function_inputs(node, node_definition, context, maybe_generate_module);
    RETURN_IF_UNEXPECTED(function_inputs);

    auto const value_inputs = gen_value_inputs(node, context, maybe_generate_module);
    RETURN_IF_UNEXPECTED(value_inputs);

    auto const func_name = base_function_name(node_definition, id);

    auto func_implementation = gen_function_definition({
        .signature_as_string = node_definition.main_function().signature_as_string,
        .unique_name         = func_name,
        .before_function     = value_inputs->code + node_definition.helper_glsl_code(),
        .body                = node_definition.main_function().body,
    });

    // Add a "namespace" to all the names that this function has defined globally (like its value inputs) so that names don't clash with another instance of the same node.
    replace_value_inputs_names(func_implementation, node.value_inputs(), value_inputs->real_names);
    replace_function_inputs_names(func_implementation, function_inputs->real_names);
    replace_output_indices_names(func_implementation, node);
    replace_names_in_global_scope(func_implementation, node_definition.names_in_global_scope(), reg::to_string(id));

    RETURN_IF_ERROR(check_there_are_no_single_quotes_left(func_implementation, list_all_input_and_output_names(node.value_inputs(), node_definition.function_inputs(), node_definition.output_indices())));

    context.push_function({.name = func_name, .definition = func_implementation});
    return func_name;
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
vec2 {}()
{{
    return vec2({}, 1.); // Convert float to float_and_alpha
}}
)STR",
            func_name, output_name
        ),
    });
}

auto gen_desired_function(
    FunctionSignature          desired_signature,
    Cool::InputPin const&      pin,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module,
    bool                       fallback_to_a_default_function
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
        maybe_generate_module,
        fallback_to_a_default_function
    );
}

auto gen_desired_function(
    FunctionSignature          desired_signature,
    Cool::NodeId const&        id,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module,
    bool                       fallback_to_a_default_function
) -> ExpectedFunctionName
{
    auto const maybe_node = context.graph().try_get_node<Node>(id);

    return gen_desired_function(
        desired_signature,
        maybe_node,
        id,
        context,
        maybe_generate_module,
        fallback_to_a_default_function
    );
}

auto gen_desired_function(
    FunctionSignature          desired_signature,
    Node const*                maybe_node,
    Cool::NodeId const&        id,
    CodeGenContext&            context,
    MaybeGenerateModule const& maybe_generate_module,
    bool                       fallback_to_a_default_function
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
        maybe_generate_module
    );
}

static auto make_node_that_reads_module_texture() -> Node
{
    return Node{
        Cool::NodeDefinitionIdentifier{
            .definition_name = "read_module_texture",
            .category_name   = "read_module_texture"
        },
        0, 0
    };
}

static auto make_node_definition_that_reads_module_texture(std::string const& texture_name) -> NodeDefinition
{
    using fmt::literals::operator""_a;

    auto const main_function = MainFunction{
        .signature_as_string = FunctionSignatureAsString{
            .return_type    = "/* sRGB_StraightA */ vec4",
            .name           = "read_module_texture",
            .arguments_list = "/* UV */ vec2 uv",
        },
        .argument_names = {"uv"},
        .signature      = FunctionSignature{
                 .from  = PrimitiveType::UV,
                 .to    = PrimitiveType::sRGB_StraightA,
                 .arity = 1,
        },
        .body = fmt::format(R"glsl(
                    uv = unnormalize_uv(to_view_space(uv));
                    return texture({texture_name}, uv);)glsl",
                            "texture_name"_a = texture_name),
    };
    return NodeDefinition::make(
               NodeDefinition_Data{
                   .main_function         = main_function,
                   .helper_glsl_code      = {},
                   .names_in_global_scope = {},
                   .input_functions       = {},
                   .input_values          = {},
                   .output_indices        = {},
               },
               {}
    )
        .value(); // We are creating the node definition from scratch, so we know that it is valid.
}

auto gen_desired_function(
    FunctionSignature                  desired_signature,
    std::reference_wrapper<Node const> node,
    Cool::NodeId const&                id,
    CodeGenContext&                    context,
    MaybeGenerateModule const&         maybe_generate_module
) -> ExpectedFunctionName
{
    auto const* node_definition = context.get_node_definition(node.get().id_names()); // NOLINT(readability-qualified-auto)
    if (!node_definition)
        return tl::make_unexpected(fmt::format(
            "Node definition \"{}\" was not found. Are you missing a file in your nodes folder?",
            node.get().definition_name()
        ));

    auto const maybe_module_texture_name = maybe_generate_module(id, *node_definition);

    auto new_node            = Node{};
    auto new_node_definition = NodeDefinition{};
    if (maybe_module_texture_name.has_value()) // We need to replace the current node with a fake node that reads an image from the given texture
    {
        new_node            = make_node_that_reads_module_texture();
        new_node_definition = make_node_definition_that_reads_module_texture(maybe_module_texture_name.value());
        node_definition     = &new_node_definition;
        node                = new_node; // Make the reference wrapper point to our new node
    }

    auto const base_function_name = gen_base_function(node, *node_definition, id, context, maybe_generate_module);
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
        maybe_generate_module
    );
    if (!func_body)
        return tl::make_unexpected(fmt::format(
            "Failed to generate conversion code for node \"{}\":\n{}",
            node_definition->name(), func_body.error()
        ));

    auto const func_name       = desired_function_name(*node_definition, id, desired_signature);
    auto const func_definition = gen_function_definition({
        .signature_as_string = signature_as_string(desired_signature),
        .unique_name         = func_name,
        .before_function     = "",
        .body                = *func_body,
    });

    context.push_function({.name = func_name, .definition = func_definition});
    return func_name;
}

} // namespace Lab
