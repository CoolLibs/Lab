#include "parse_node_definition.h"
#include <Cool/Expected/RETURN_IF_UNEXPECTED.h>
#include <Cool/String/String.h>
#include "Module_Nodes/FunctionSignature.h"
#include "Module_Nodes/NodeDefinition.h"
#include "Module_Nodes/PrimitiveType.h"
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

struct TypeAndArity {
    PrimitiveType type;
    size_t        arity;
};

auto parse_parameters_types(std::string const& parameters_list)
    -> tl::expected<TypeAndArity, std::string>
{
    auto const first_type_pos = Cool::String::find_next_word_position(parameters_list, 0);
    if (!first_type_pos)
        return TypeAndArity{
            .type  = PrimitiveType::Void,
            .arity = 0,
        };

    auto const first_type = parse_primitive_type(Cool::String::substring(parameters_list, *first_type_pos));
    RETURN_IF_UNEXPECTED(first_type);

    auto const first_name_pos = Cool::String::find_next_word_position(parameters_list, first_type_pos->second);
    if (!first_name_pos)
        return tl::make_unexpected("Missing parameter name.");

    size_t arity{1};
    auto   type_pos = Cool::String::find_next_word_position(parameters_list, first_name_pos->second);
    while (type_pos)
    {
        arity++;

        auto const type = parse_primitive_type(Cool::String::substring(parameters_list, *type_pos));
        RETURN_IF_UNEXPECTED(type);
        if (type != first_type)
            return tl::make_unexpected(fmt::format( // TODO(JF) This is only true for the main function. Don't error for helper functions.
                "Functions cannot have different parameters types. Found {} and {}.",
                cpp_type_as_string(*first_type),
                cpp_type_as_string(*type)
            ));

        auto const name_pos = Cool::String::find_next_word_position(parameters_list, type_pos->second);
        if (!name_pos)
            return tl::make_unexpected("Missing parameter name.");

        type_pos = Cool::String::find_next_word_position(parameters_list, name_pos->second);
    }

    return TypeAndArity{
        .type  = *first_type,
        .arity = arity,
    };
}

struct parse_signature_params {
    std::string const& parameters_list;
    std::string const& output_type;
};

static auto parse_signature(parse_signature_params p)
    -> tl::expected<FunctionSignature, std::string>
{
    auto res = FunctionSignature{};

    auto const output_type = parse_primitive_type(p.output_type);
    RETURN_IF_UNEXPECTED(output_type);
    res.to = *output_type;

    auto const parameters_types = parse_parameters_types(p.parameters_list);
    RETURN_IF_UNEXPECTED(parameters_types);
    res.from  = parameters_types->type;
    res.arity = parameters_types->arity;

    return res;
}

struct NameAndSignature {
    std::string       name;
    FunctionSignature signature;
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

auto parse_node_definition(std::string const& name, std::string text)
    -> tl::expected<NodeDefinition_Data, std::string>
{
    NodeDefinition_Data res{};

    text = Cool::String::remove_comments(text);

    auto functions = parse_all_functions(text);
    if (!functions)
        return tl::make_unexpected(functions.error());

    auto const main_function_it = std::find_if(functions->begin(), functions->end(), [](FunctionPieces const& func) {
        return func.name == "main";
    });
    if (main_function_it == functions->end())
        return tl::make_unexpected("Missing a main function.");

    res.main_function      = *main_function_it;
    res.main_function.name = name;
    functions->erase(main_function_it);
    res.helper_functions = *functions;

    return res;
}

} // namespace Lab