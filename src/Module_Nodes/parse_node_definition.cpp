#include "parse_node_definition.h"
#include <Cool/Expected/RETURN_IF_UNEXPECTED.h>
#include <Cool/String/String.h>
#include <algorithm>
#include <exception>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include "Cool/Dependencies/InputDefinition.h"
#include "Cool/Log/Debug.h"
#include "Cool/type_from_string/type_from_string.h"
#include "Debug/DebugOptions.h"
#include "Module_Nodes/FunctionSignature.h"
#include "Module_Nodes/NodeDefinition.h"
#include "Module_Nodes/PrimitiveType.h"
#include "stringify/stringify.hpp"
#include "tl/expected.hpp"

namespace Lab {

static auto parse_primitive_type(std::string const& str)
    -> tl::expected<PrimitiveType, std::string>
{
#include "generated/parse_primitive_type.inl"

    return tl::make_unexpected(fmt::format(
        "Unknown type '{}'. The available types are:\n{}",
        str,
#include "generated/string_listing_the_parsed_types.inl"
    ));
}

static auto parse_parameters(std::string const& parameters_list)
    -> tl::expected<ParametersList, std::string>
{
    auto params = ParametersList{};

    auto const first_type_pos = Cool::String::find_next_word_position(parameters_list, 0);
    if (!first_type_pos)
        return params;

    auto const first_type = parse_primitive_type(Cool::String::substring(parameters_list, *first_type_pos));
    RETURN_IF_UNEXPECTED(first_type);

    auto const first_name_pos = Cool::String::find_next_word_position(parameters_list, first_type_pos->second);
    if (!first_name_pos)
        return tl::make_unexpected("Missing parameter name.");

    params.push_back(ParamDesc{
        .name = Cool::String::substring(parameters_list, *first_name_pos),
        .type = *first_type,
    });

    auto type_pos = Cool::String::find_next_word_position(parameters_list, first_name_pos->second);
    while (type_pos)
    {
        auto const type = parse_primitive_type(Cool::String::substring(parameters_list, *type_pos));
        RETURN_IF_UNEXPECTED(type);

        auto const name_pos = Cool::String::find_next_word_position(parameters_list, type_pos->second);
        if (!name_pos)
            return tl::make_unexpected("Missing parameter name.");

        params.push_back(ParamDesc{
            .name = Cool::String::substring(parameters_list, *name_pos),
            .type = *type,
        });

        type_pos = Cool::String::find_next_word_position(parameters_list, name_pos->second);
    }

    return params;
}

struct parse_signature_params {
    std::string const& parameters_list;
    std::string const& output_type;
};

static auto
    parse_signature(parse_signature_params p)
        -> tl::expected<CompleteFunctionSignature, std::string>
{
    auto res = CompleteFunctionSignature{};

    auto const output_type = parse_primitive_type(p.output_type);
    RETURN_IF_UNEXPECTED(output_type);
    res.output_type = *output_type;

    auto const parameters = parse_parameters(p.parameters_list);
    RETURN_IF_UNEXPECTED(parameters);
    res.parameters = *parameters;

    return res;
}

struct NameAndSignature {
    std::string               name;
    CompleteFunctionSignature signature;
};

static auto parse_name_and_signature(std::string const& text, size_t end_of_function_declaration)
    -> tl::expected<NameAndSignature, std::string>
{
    auto const params_pos = Cool::String::rfind_matching_pair({
        .text    = text,
        .offset  = end_of_function_declaration,
        .opening = '(',
        .closing = ')',
    });
    if (!params_pos)
        return tl::make_unexpected("Did not find the parameters of the function. You are missing a parenthesis.");

    auto const name_pos = Cool::String::find_previous_word_position(text, params_pos->first);
    if (!name_pos)
        return tl::make_unexpected("Did not find the name of the function.");

    auto const output_type_pos = Cool::String::find_previous_word_position(text, name_pos->first);
    if (!output_type_pos)
        return tl::make_unexpected("Did not find the output type of the function.");

    auto const signature = parse_signature({
        .parameters_list = Cool::String::substring(text, *params_pos),
        .output_type     = Cool::String::substring(text, *output_type_pos),
    });
    RETURN_IF_UNEXPECTED(signature);

    return NameAndSignature{
        .name      = Cool::String::substring(text, *name_pos),
        .signature = *signature,
    };
}

static auto parse_all_functions(std::string text)
    -> tl::expected<std::vector<FunctionPieces>, std::string>
{
    auto res = std::vector<FunctionPieces>{};

    auto const get_next_bracket_pos = [&](size_t offset) {
        return Cool::String::find_matching_pair({
            .text    = text,
            .offset  = offset,
            .opening = '{',
            .closing = '}',
        });
    };

    auto brackets_pos = get_next_bracket_pos(0);
    while (brackets_pos)
    {
        auto function = FunctionPieces{};

        function.body = Cool::String::substring(text, brackets_pos->first + 1, brackets_pos->second);

        auto const name_and_sig = parse_name_and_signature(text, brackets_pos->first);
        RETURN_IF_UNEXPECTED(name_and_sig);
        function.name      = name_and_sig->name;
        function.signature = name_and_sig->signature;

        res.push_back(function);
        brackets_pos = get_next_bracket_pos(brackets_pos->second);
    }

    return res;
}

static auto make_main_function_signature(CompleteFunctionSignature const& signature)
    -> tl::expected<MainFunctionSignature, std::string>
{
    auto res = MainFunctionSignature{};

    res.signature.to = signature.output_type;

    res.signature.arity = signature.parameters.size();

    res.signature.from = signature.parameters.empty()
                             ? PrimitiveType::Void
                             : signature.parameters[0].type;

    for (auto const& param : signature.parameters)
    {
        if (param.type != res.signature.from)
            return tl::make_unexpected(fmt::format(
                "The main function cannot have different parameters types. Found {} and {}.\nIf you need more parameters, consider using an INPUT instead.",
                cpp_type_as_string(res.signature.from),
                cpp_type_as_string(param.type)
            ));
    }

    std::transform(
        signature.parameters.begin(), signature.parameters.end(),
        std::back_inserter(res.parameter_names),
        [](ParamDesc const& param) {
            return param.name;
        }
    );

    return res;
}

static auto make_main_function_pieces(FunctionPieces const& pieces, std::string const& name)
    -> tl::expected<MainFunctionPieces, std::string>
{
    auto res = MainFunctionPieces{};

    res.name = name;
    res.body = pieces.body;

    auto const sig = make_main_function_signature(pieces.signature);
    RETURN_IF_UNEXPECTED(sig);
    res.signature = *sig;

    return res;
}

static auto check_that_helper_functions_dont_use_the_any_type(std::vector<FunctionPieces> const& functions)
    -> std::optional<std::string>
{
    for (auto const& function : functions)
    {
        if (
            function.signature.output_type == PrimitiveType::Any
            || std::any_of(function.signature.parameters.begin(), function.signature.parameters.end(), [](ParamDesc const& param) {
                   return param.type == PrimitiveType::Any;
               })
        )
        {
            return fmt::format("The Any type is only allowed for the main function. You cannot use it in {}.", function.name);
        }
    }
    return std::nullopt;
}

/// HACK to apply pre-divide / post-multiply to rgb post-process effects
static auto convert_rgb_transform_to_rgba(NodeDefinition_Data definition)
    -> NodeDefinition_Data
{
    // TODO(JF)
    return definition;
    //     if (definition.main_function.signature.signature != Signature::RGBTransformation)
    //         return definition;

    //     auto const base_name = fmt::format("RGB{}", definition.main_function.name);
    //     definition.helper_functions.push_back(FunctionPieces{
    //         .name      = base_name,
    //         .signature = make_complete_function_signature(definition.main_function.signature),
    //         .body      = definition.main_function.body,
    //     });

    //     definition.main_function.signature.signature = Signature::RGBATransformation;
    //     definition.main_function.body                = fmt::format(
    //         R"STR(
    //     if (in1.a < 0.000001)
    //         return in1;
    //     vec3 rgb = in1.rgb / in1.a;
    //     rgb      = {}(rgb);
    //     return vec4(rgb * in1.a, in1.a);
    // )STR",
    //         base_name
    //     );

    //     return definition;
}

static auto find_main_and_helper_functions(std::filesystem::path const& filepath, std::string const& text, NodeDefinition_Data& res)
    -> std::optional<std::string>
{
    auto functions = parse_all_functions(text);
    if (!functions)
        return functions.error();

    auto const main_function_it = std::find_if(functions->begin(), functions->end(), [](FunctionPieces const& func) {
        return func.name == "main";
    });
    if (main_function_it == functions->end())
        return "Missing a main function.";

    auto const main_function = make_main_function_pieces(*main_function_it, filepath.stem().string());
    if (!main_function)
        return main_function.error();
    res.main_function = *main_function;

    functions->erase(main_function_it);
    res.helper_functions = *functions;
    {
        auto const err = check_that_helper_functions_dont_use_the_any_type(*functions);
        if (err)
            return *err;
    }

    return std::nullopt;
}

static auto parse_signature(std::vector<std::string> const& words)
    -> tl::expected<FunctionSignature, std::string>
{
    assert(words.size() >= 2);

    auto const output_type = parse_primitive_type(words.back());
    if (!output_type)
        return tl::make_unexpected(output_type.error());

    auto const input_type = parse_primitive_type(words.front());
    if (!input_type)
        return tl::make_unexpected(input_type.error());

    // Error checking
    if (input_type == PrimitiveType::Void)
        return tl::make_unexpected("'Void' is not allowed as an INPUT type.");
    if (input_type == PrimitiveType::Any)
        return tl::make_unexpected("'Any' is not allowed as an INPUT type.");
    for (size_t i = 1; i < words.size() - 1; ++i)
    {
        auto const other_input_type = parse_primitive_type(words[i]);
        if (!other_input_type)
            return tl::make_unexpected(other_input_type.error());
        if (*input_type != *other_input_type)
            return tl::make_unexpected(fmt::format(
                "INPUT functions cannot have different parameters types. Found {} and {}.",
                cpp_type_as_string(*input_type),
                cpp_type_as_string(*other_input_type)
            ));
    }

    return FunctionSignature{
        .from  = *input_type,
        .to    = *output_type,
        .arity = words.size() - 1,
    };
}

static auto parse_property(std::string const& type_as_string, std::string const& name, NodeDefinition_Data& res)
    -> std::optional<std::string>
{
    try
    {
        res.properties.emplace_back(COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(
            Cool::InputDefinition,
            type_as_string,
            Cool::AnyInputDefinition,
            (name)
        ));
    }
    catch (std::exception const& e)
    {
        return e.what();
    }

    return std::nullopt;
}

static auto parse_input(std::vector<std::string> const& type_words, std::string const& name, NodeDefinition_Data& res)
    -> std::optional<std::string>
{
    auto const signature = parse_signature(type_words);
    if (!signature)
        return signature.error();

    res.inputs.emplace_back(NodeInputDefinition_Data{
        .name      = name,
        .signature = *signature,
    });

    return std::nullopt;
}

template<typename Callable>
static auto find_declaration(std::string const& text, std::string_view keyword, Callable&& handle_type_words)
    -> std::optional<std::string>
{
    size_t offset = text.find(keyword);
    while (offset != std::string::npos)
    {
        offset += keyword.length();
        auto const end_of_line = text.find(';', offset);

        auto const error_message = [&](std::string_view str) {
            return fmt::format("Invalid {} declaration: {}. While reading:\n{}", keyword, str, Cool::String::substring(text, offset - keyword.length(), end_of_line != std::string::npos ? end_of_line + 1 : text.length()));
        };

        if (end_of_line == std::string::npos)
            return error_message("missing semicolon (;)");

        auto const name_pos = Cool::String::find_matching_pair({
            .text    = text,
            .offset  = offset,
            .opening = '`',
            .closing = '`',
        });
        if (!name_pos || name_pos->second > end_of_line)
            return error_message("missing name. A name must start and end with backticks (`)");

        auto const type_words = Cool::String::all_words(Cool::String::substring(text, offset, name_pos->first));

        if (type_words.empty())
            return error_message("missing type");

        {
            auto const name = Cool::String::substring(text, name_pos->first, name_pos->second + 1);

            auto const err = handle_type_words(type_words, name);
            if (err)
                return error_message(fmt::format("{}\n", *err));
        }

        offset = text.find(keyword, end_of_line + 1);
    }

    return std::nullopt;
}

static auto find_inputs_and_properties(std::string const& text, NodeDefinition_Data& res)
    -> std::optional<std::string>
{
    return find_declaration(
        text,
        "INPUT",
        [&](std::vector<std::string> const& type_words, std::string const& name) -> std::optional<std::string> {
            return type_words.size() == 1
                       ? parse_property(type_words[0], name, res)
                       : parse_input(type_words, name, res);
        }
    );
}

static auto find_outputs(std::string const& text, NodeDefinition_Data& res)
    -> std::optional<std::string>
{
    return find_declaration(
        text,
        "OUTPUT",
        [&](std::vector<std::string> const& type_words, std::string const& name) -> std::optional<std::string> {
            if (type_words.size() > 1)
                return fmt::format("too many words; expected a type declaration, got {}", Cool::stringify(type_words));
            if (type_words[0] != "float")
                return fmt::format("invalid type. 'float' is the only allowed OUTPUT type. Found '{}'", type_words[0]);
            res.output_indices.emplace_back(name);
            return std::nullopt;
        }
    );
}

auto parse_node_definition(std::filesystem::path filepath, std::string text)
    -> tl::expected<NodeDefinition, std::string>
{
#if DEBUG
    if (DebugOptions::log_when_parsing_node_definition())
        Cool::Log::Debug::info("Nodes", fmt::format("Parsing node definition from {}.", filepath));
#endif

    NodeDefinition_Data res{};

    text = Cool::String::remove_comments(text);

    {
        auto const err = find_main_and_helper_functions(filepath, text, res);
        if (err)
            return tl::make_unexpected(*err);
    }
    {
        auto const err = find_inputs_and_properties(text, res);
        if (err)
            return tl::make_unexpected(*err);
    }
    {
        auto const err = find_outputs(text, res);
        if (err)
            return tl::make_unexpected(*err);
    }

    filepath += ".presets.json";
    return NodeDefinition::make(
        convert_rgb_transform_to_rgba(res),
        filepath
    );
}

} // namespace Lab