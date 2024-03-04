#include "parse_node_definition.h"
#include <Cool/Expected/RETURN_IF_ERROR.h>
#include <Cool/Expected/RETURN_IF_UNEXPECTED.h>
#include <Cool/RegExp/RegExp.h>
#include <Cool/String/String.h>
#include <algorithm>
#include <exception>
#include <filesystem>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Dependencies/SharedVariableDefinition.h"
#include "Cool/Variables/PresetManager.h"
#include "Cool/type_from_string/type_from_string.h"
#include "Debug/DebugOptions.h"
#include "FunctionSignature.h"
#include "NodeDefinition.h"
#include "PrimitiveType.h"
#include "fixup_node_definition.h"
#include "stringify/stringify.hpp"
#include "tl/expected.hpp"

namespace Lab {

// static auto decompose_signature_string(std::string const& text, size_t end_of_function_declaration)
//     -> tl::expected<FunctionSignatureAsString, std::string>
// {
//     auto args_pos = Cool::String::rfind_matching_pair({
//         .text    = text,
//         .offset  = end_of_function_declaration,
//         .opening = '(',
//         .closing = ')',
//     });
//     if (!args_pos)
//         return tl::make_unexpected("Did not find the arguments of the function. You are missing a parenthesis.");
//     args_pos->first += 1; // Skip the first parenthesis

//     auto const name_pos = Cool::String::find_previous_word_position(text, args_pos->first);
//     if (!name_pos)
//         return tl::make_unexpected("Did not find the name of the function.");

//     auto const return_type_pos = Cool::String::find_previous_word_position(text, name_pos->first);
//     if (!return_type_pos)
//         return tl::make_unexpected("Did not find the return type of the function.");

//     return FunctionSignatureAsString{
//         .return_type    = Cool::String::substring(text, *return_type_pos),
//         .name           = Cool::String::substring(text, *name_pos),
//         .arguments_list = Cool::String::substring(text, *args_pos),
//     };
// }

// static auto parse_all_functions_and_structs(std::string text)
//     -> tl::expected<std::pair<std::vector<Function>, std::vector<Struct>>, std::string>
// {
//     auto functions = std::vector<Function>{};
//     auto structs   = std::vector<Struct>{};

//     auto const get_next_bracket_pos = [&](size_t offset) {
//         return Cool::String::find_matching_pair({
//             .text    = text,
//             .offset  = offset,
//             .opening = '{',
//             .closing = '}',
//         });
//     };

//     auto brackets_pos = get_next_bracket_pos(0);
//     while (brackets_pos)
//     {
//         auto const body = Cool::String::substring(text, brackets_pos->first, brackets_pos->second + 1);
//         // Check if it is a struct or a function
//         auto const struct_name_pos = Cool::String::find_previous_word_position(text, brackets_pos->first);
//         if (!struct_name_pos)
//         {
//             brackets_pos = {};
//             continue;
//         }
//         auto const struct_keyword_pos = Cool::String::find_previous_word_position(text, struct_name_pos->first);
//         // Parse struct
//         if (struct_keyword_pos && Cool::String::substring(text, *struct_keyword_pos) == "struct")
//         {
//             structs.push_back(Struct{
//                 .name = Cool::String::substring(text, *struct_name_pos),
//                 .body = body,
//             });
//             std::cout << body << '\n';
//         }
//         // Parse function
//         else
//         {
//             auto function = Function{};

//             function.body = body;

//             auto const signature_as_string = decompose_signature_string(text, brackets_pos->first);
//             RETURN_IF_UNEXPECTED(signature_as_string);
//             function.signature_as_string = *signature_as_string;

//             functions.push_back(function);
//         }
//         brackets_pos = get_next_bracket_pos(brackets_pos->second);
//     }

//     return std::make_pair(functions, structs);
// }

// static auto parse_primitive_type(std::string const& str) // NOLINT(readability-function-cognitive-complexity)
//     -> tl::expected<PrimitiveType, std::string>
// {
// #include "generated/parse_primitive_type.inl"

//     return tl::make_unexpected(fmt::format(
//         "Unknown type '{}'. The available types are:\n{}",
//         str,
// #include "generated/string_listing_the_parsed_types.inl"
//     ));
// }

// struct Argument {
//     PrimitiveType type;
//     std::string   name;
// };

// static auto parse_arguments(std::string const& arguments_list)
//     -> tl::expected<std::vector<Argument>, std::string>
// {
//     auto args = std::vector<Argument>{};

//     auto type_pos = Cool::String::find_next_word_position(arguments_list, 0);
//     while (type_pos)
//     {
//         auto const type = parse_primitive_type(Cool::String::substring(arguments_list, *type_pos));
//         RETURN_IF_UNEXPECTED(type);

//         auto const name_pos = Cool::String::find_next_word_position(arguments_list, type_pos->second);
//         if (!name_pos)
//             return tl::make_unexpected("Missing argument name in main function.");

//         args.push_back(Argument{
//             .type = *type,
//             .name = Cool::String::substring(arguments_list, *name_pos),
//         });

//         type_pos = Cool::String::find_next_word_position(arguments_list, name_pos->second);
//     }

//     return args;
// }

// // static auto parse_function_signature(FunctionSignatureAsString const& as_str)
// //     -> tl::expected<FunctionSignature, std::string>
// // {
// //     auto signature = FunctionSignature{};

// //     { // Parse return type
// //         auto const return_type = parse_primitive_type(as_str.return_type);
// //         RETURN_IF_UNEXPECTED(return_type);
// //         signature.to = *return_type;
// //     }

// //     { // Parse arguments list
// //         auto const arguments = parse_arguments(as_str.arguments_list);
// //         RETURN_IF_UNEXPECTED(arguments);

// //         signature.arity = arguments->size();
// //         signature.from  = arguments->empty()
// //                               ? PrimitiveType::Void
// //                               : (*arguments)[0].type;

// //         // Check that we only have one argument type
// //         for (auto const& arg : *arguments)
// //         {
// //             if (arg.type != (*arguments)[0].type)
// //             {
// //                 return tl::make_unexpected(fmt::format(
// //                     "The main function cannot have different arguments types. Found {} and {}.\nIf you need more arguments, consider using an INPUT instead.",
// //                     cpp_type_as_string((*arguments)[0].type),
// //                     cpp_type_as_string(arg.type)
// //                 ));
// //             }
// //         }
// //     }

// //     return signature;
// // }

// static auto gen_arguments_list(std::vector<Argument> const& args)
//     -> std::string
// {
//     using fmt::literals::operator""_a;

//     std::string res{};
//     for (size_t i = 0; i < args.size(); ++i)
//     {
//         res += fmt::format(
//             FMT_COMPILE("{type} {name}"),
//             "type"_a = glsl_type_as_string(args[i].type),
//             "name"_a = args[i].name
//         );
//         if (i != args.size() - 1)
//             res += ", ";
//     }

//     return res;
// }

// static auto make_main_function(Function const& function, std::string const& name)
//     -> tl::expected<MainFunction, std::string>
// {
//     auto main = MainFunction{};

//     main.function                          = function;
//     main.function.signature_as_string.name = name; // Change name, we don't want it to be "main", but the name of the node

//     { // Parse return type
//         auto const return_type = parse_primitive_type(function.signature_as_string.return_type);
//         RETURN_IF_UNEXPECTED(return_type);
//         main.signature.to                             = *return_type;
//         main.function.signature_as_string.return_type = glsl_type_as_string(*return_type); // Change the return type to a regular glsl type so that it is valid when copy-pasted into shader code.
//     }

//     { // Parse arguments list
//         auto const arguments = parse_arguments(function.signature_as_string.arguments_list);
//         RETURN_IF_UNEXPECTED(arguments);

//         main.signature.arity = arguments->size();
//         main.signature.from  = arguments->empty()
//                                    ? PrimitiveType::Void
//                                    : (*arguments)[0].type;

//         main.function.signature_as_string.arguments_list = gen_arguments_list(*arguments); // Change arguments list so that it only uses regular glsl types, just like normal helper functions. Otherwise would generate invalid glsl code when generating the code for the function

//         for (auto const& arg : *arguments)
//         {
//             main.argument_names.push_back(arg.name);
//             // Check that we only have one argument type
//             if (arg.type != (*arguments)[0].type)
//             {
//                 return tl::make_unexpected(fmt::format(
//                     "The main function cannot have different arguments types. Found {} and {}.\nIf you need more arguments, consider using an INPUT instead.",
//                     cpp_type_as_string((*arguments)[0].type),
//                     cpp_type_as_string(arg.type)
//                 ));
//             }
//         }
//     }

//     return main;
// }

// static auto find_main_functions(std::filesystem::path const& filepath, std::string const& text, NodeDefinition_Data& res)
//     -> std::optional<std::string>
// {
//     auto functions_and_structs = parse_all_functions_and_structs(text);
//     if (!functions_and_structs)
//         return functions_and_structs.error();

//     {
//         auto& functions = functions_and_structs->first;

//         auto const main_function_it = std::find_if(functions.begin(), functions.end(), [](Function const& func) {
//             return func.signature_as_string.name == "main";
//         });
//         if (main_function_it == functions.end())
//             return "Missing a main function.";

//         auto const main_function = make_main_function(*main_function_it, filepath.stem().string());
//         if (!main_function)
//             return main_function.error();
//         res.main_function = *main_function;
//     }

//     return std::nullopt;
// }

// static auto parse_input_function_signature(std::vector<std::string> const& type_words)
//     -> tl::expected<FunctionSignature, std::string>
// {
//     assert(type_words.size() >= 2);

//     auto const return_type = parse_primitive_type(type_words.back());
//     RETURN_IF_UNEXPECTED(return_type);

//     auto const input_type = parse_primitive_type(type_words.front());
//     RETURN_IF_UNEXPECTED(input_type);

//     // Error checking
//     if (input_type == PrimitiveType::Void)
//         return tl::make_unexpected("'Void' is not allowed as an argument type for INPUT functions.");
//     for (size_t i = 1; i < type_words.size() - 1; ++i)
//     {
//         auto const other_input_type = parse_primitive_type(type_words[i]);
//         if (!other_input_type)
//             return tl::make_unexpected(other_input_type.error());
//         if (*input_type != *other_input_type)
//             return tl::make_unexpected(fmt::format(
//                 "INPUT functions cannot have different arguments types. Found {} and {}.",
//                 cpp_type_as_string(*input_type),
//                 cpp_type_as_string(*other_input_type)
//             ));
//     }

//     return FunctionSignature{
//         .from  = *input_type,
//         .to    = *return_type,
//         .arity = type_words.size() - 1,
//     };
// }

// template<typename T>
// static auto make_shared_var_definition(std::string const& name, std::string const& type) -> Cool::AnySharedVariableDefinition
// {
//     auto def = Cool::SharedVariableDefinition<T>{{name}};

//     if constexpr (std::is_same_v<T, Cool::Color>)
//     {
//         def.desired_color_space = static_cast<int>(Cool::parse_color_space(type));
//     }
//     else if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
//     {
//         def.desired_color_space = static_cast<int>(Cool::parse_color_and_alpha_space(type));
//     }

//     return def;
// }

// static auto parse_input_value(std::string const& type_as_string, std::string const& name, NodeDefinition_Data& res)
//     -> std::optional<std::string>
// {
//     try
//     {
//         res.input_values.emplace_back(COOL_TFS_EVALUATE_FUNCTION_TEMPLATE(
//             make_shared_var_definition,
//             type_as_string,
//             Cool::AnySharedVariableDefinition,
//             (name, type_as_string)
//         ));
//     }
//     catch (std::exception const& e)
//     {
//         return e.what();
//     }

//     return std::nullopt;
// }

// static auto parse_input_function(std::vector<std::string> const& type_words, std::string const& name, NodeDefinition_Data& res)
//     -> std::optional<std::string>
// {
//     auto const signature = parse_input_function_signature(type_words);
//     if (!signature)
//         return signature.error();

//     res.input_functions.emplace_back(NodeInputDefinition_Data{
//         .name      = name,
//         .signature = *signature,
//     });

//     return std::nullopt;
// }

// template<typename Callback>
// static auto find_declaration(std::string& text, std::string_view keyword, Callback const& handle_type_words)
//     -> std::optional<std::string>
// {
//     size_t offset = text.find(keyword);
//     while (offset != std::string::npos)
//     {
//         auto const start_of_declaration = offset;
//         offset += keyword.length();
//         auto const end_of_line = text.find(';', offset);

//         auto const error_message = [&](std::string_view str) {
//             return fmt::format("Invalid {} declaration: {}. While reading:\n{}", keyword, str, Cool::String::substring(text, offset - keyword.length(), end_of_line != std::string::npos ? end_of_line + 1 : text.length()));
//         };

//         if (end_of_line == std::string::npos)
//             return error_message("missing semicolon (;)");

//         auto const name_pos = Cool::String::find_matching_pair({
//             .text    = text,
//             .offset  = offset,
//             .opening = '\'',
//             .closing = '\'',
//         });
//         if (!name_pos || name_pos->second > end_of_line)
//             return error_message("missing name. A name must start and end with single quotes (')");

//         auto const type_words = Cool::String::all_words(Cool::String::substring(text, offset, name_pos->first));

//         if (type_words.empty())
//             return error_message("missing type");

//         {
//             auto const name = Cool::String::substring(text, name_pos->first, name_pos->second + 1);

//             auto const err = handle_type_words(type_words, name);
//             if (err)
//                 return error_message(fmt::format("{}\n", *err));
//         }
//         Cool::String::remove_substring(text, start_of_declaration, end_of_line + 1);
//         offset = text.find(keyword, start_of_declaration);
//     }

//     return std::nullopt;
// }

// static auto find_inputs(std::string& text, NodeDefinition_Data& res)
//     -> std::optional<std::string>
// {
//     return find_declaration(
//         text,
//         "INPUT",
//         [&](std::vector<std::string> const& type_words, std::string const& name) -> std::optional<std::string> {
//             return type_words.size() == 1
//                        ? parse_input_value(type_words[0], name, res)
//                        : parse_input_function(type_words, name, res);
//         }
//     );
// }

// static auto find_outputs(std::string& text, NodeDefinition_Data& res)
//     -> std::optional<std::string>
// {
//     return find_declaration(
//         text,
//         "OUTPUT",
//         [&](std::vector<std::string> const& type_words, std::string const& name) -> std::optional<std::string> {
//             if (type_words.size() > 1)
//                 return fmt::format("too many words; expected a type declaration, got {}", Cool::stringify(type_words));
//             if (type_words[0] != "float")
//                 return fmt::format("invalid type. 'float' is the only allowed OUTPUT type. Found '{}'", type_words[0]);
//             res.output_indices.emplace_back(name);
//             return std::nullopt;
//         }
//     );
// }

// template<typename T>
// static void maybe_set_input_description(std::string const& input_name, Cool::SharedVariableDefinition<T>& def, std::string const& description)
// {
//     if (def.var_data.name == input_name)
//         def.description = description;
// }

// static void set_input_description(NodeDefinition_Data& res, std::string const& input_name, std::string const& input_description)
// {
//     for (auto& input : res.input_values)
//     {
//         std::visit([&](auto&& input) { maybe_set_input_description(input_name, input, input_description); }, input);
//     }

//     for (auto& input : res.input_functions)
//     {
//         if ("'" + input.name() + "'" == input_name)
//             input.set_description(input_description);
//     }
// }

// static void find_inputs_descriptions(std::string const& text, NodeDefinition_Data& res)
// {
//     size_t desc_offset = 0;
//     size_t name_offset = 0;

//     auto pos_of_the_input_name = std::make_optional<std::pair<size_t, size_t>>(); // Dummy value. We just need it to not be nullopt in order to enter the while loop, but it will be properly computed at the very beginning of that while loop.
//     while (pos_of_the_input_name)
//     {
//         // Update positions
//         pos_of_the_input_name = Cool::String::find_matching_pair({
//             .text    = text,
//             .offset  = name_offset,
//             .opening = '\'',
//             .closing = '\'',
//         });
//         if (!pos_of_the_input_name)
//             break;

//         auto const pos_of_text_after_the_input_declaration = Cool::String::find_matching_pair({
//             .text    = text,
//             .offset  = desc_offset,
//             .opening = ';',
//             .closing = '\n',
//         });
//         if (!pos_of_text_after_the_input_declaration)
//             break;

//         auto const move_to_next_description = [&]() {
//             name_offset = pos_of_the_input_name->second + 1;
//             desc_offset = pos_of_text_after_the_input_declaration->second + 1;
//         };

//         auto const text_after_the_input_declaration = Cool::String::substring(text, *pos_of_text_after_the_input_declaration);

//         auto const triple_slash_pos = text_after_the_input_declaration.find("///");
//         if (triple_slash_pos == std::string::npos)
//         {
//             move_to_next_description();
//             continue;
//         }

//         auto const input_description = Cool::String::substring(
//             text_after_the_input_declaration,
//             triple_slash_pos + 3,
//             text_after_the_input_declaration.size()
//         );

//         pos_of_the_input_name->second = pos_of_the_input_name->second + 1; // Increase by 1 to include the last single quote (')
//         std::string const input_name  = Cool::String::substring(text, *pos_of_the_input_name);

//         set_input_description(res, input_name, input_description);

//         move_to_next_description();
//     }
// }

// static auto input_fix_artifacts()
// {
//     return Cool::SharedVariableDefinition<float>{
//         .var_data = Cool::VariableData<float>{
//             .name     = "'Fix Artifacts'",
//             .value    = 0.f,
//             .metadata = Cool::VariableMetadata<float>{
//                 .bounds = {
//                     .min           = 0.f,
//                     .max           = 0.999f,
//                     .has_min_bound = true,
//                     .has_max_bound = true,
//                     .drag_speed    = 0.01f,
//                     .use_slider    = true,
//                 },
//             },
//         },
//         .description = "Increase the value to fix glitches and holes in the shape. But note that higher values are slower to render.",
//     };
// }

// static auto parse_special_coollab_syntax_and_remove_it(
//     std::string const&           text,
//     std::string&                 text_without_comments,
//     std::filesystem::path const& filepath,
//     NodeDefinition_Data&         def
// ) -> std::optional<std::string>
// {
//     bool const fix_artifacts = text_without_comments.find("CLB_FIX_ARTIFACTS") != std::string::npos;
//     if (fix_artifacts)
//         Cool::String::replace_all_inplace(text_without_comments, "CLB_FIX_ARTIFACTS", "(1. - 'Fix Artifacts') * ");

//     RETURN_IF_ERR(find_main_function(filepath, text_without_comments, def));
//     RETURN_IF_ERR(find_inputs(text_without_comments, def));
//     RETURN_IF_ERR(find_outputs(text_without_comments, def));
//     find_inputs_descriptions(text, def); // Must be done after finding the inputs. Must work on the text WITH comments because the descriptions are inside comments.

//     if (fix_artifacts) // Done after finding the inputs, because we want this to be the last input
//         def.input_values.emplace_back(input_fix_artifacts());
// }

static auto presets_paths(std::filesystem::path path) -> Cool::PresetsPaths
{
    path += ".presets.json";
#if DEBUG
    return Cool::PresetsPaths{
        .user_defined_presets = std::move(path),
        .default_presets      = "",
    };
#else
    return Cool::PresetsPaths{
        .user_defined_presets = Cool::Path::user_data() / std::filesystem::relative(path, Cool::Path::root()), // Convert a path relative to root() into a path relative to user_data()
        .default_presets      = path,
    };
#endif
}

auto parse_node_definition(std::filesystem::path const& filepath, std::string const& text)
    -> tl::expected<NodeDefinition, std::string>
{
    auto def = NodeDefinition_Data{};
    if (DebugOptions::log_when_parsing_node_definition())
        Cool::Log::ToUser::info("Nodes", fmt::format("Parsing node definition from {}.", filepath));

    def.helper_glsl_code      = R"glsl(
    struct Bob{
        float radius;
    };

    float test(vec2 uv, Bob bob)
    {
        return length(uv) - bob.radius;
    }
)glsl";
    def.names_in_global_scope = {"Bob", "test"};
    def.main_function         = MainFunction{
                .function = Function{
                    .signature_as_string = FunctionSignatureAsString{
                        .return_type    = "float",
                        .name           = "Disk",
                        .arguments_list = "vec2 uv",
            },
                    .body = R"glsl(
                return test(uv, Bob(1.));
            )glsl",
        },
                .signature = FunctionSignature{
                    .from  = PrimitiveType::UV,
                    .to    = PrimitiveType::SignedDistance,
                    .arity = 1,
        },
                .argument_names = {"uv"},
    };
    // auto text_without_comments = Cool::String::remove_comments(text);
    // RETURN_IF_ERROR(parse_special_coollab_syntax_and_remove_it(text, text_without_comments, filepath, def)); // Leaves only regular glsl code
    // RETURN_IF_ERROR(find_names_declared_in_global_scope(text_without_comments, def));                        // That we can then check for all symbols declared in global scope (functions, structs, constants, #define, etc.)
    // fixup_node_definition(def);

    return NodeDefinition::make(def, presets_paths(filepath));
}

} // namespace Lab