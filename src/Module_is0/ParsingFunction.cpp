#include "ParsingFunction.h"

std::string find_extra_code(std::string_view source, size_t* end_pos)
{
    const auto extra_code_pos = Cool::String::find_matching_pair(source, '`', '`');
    if (!extra_code_pos.has_value()) {
        return "";
    }
    else {
        *end_pos = extra_code_pos->second + 1;
        return std::string{source.substr(extra_code_pos->first + 1, extra_code_pos->second - extra_code_pos->first - 1)};
    }
}

std::string find_parameters_declaration(std::string_view source, size_t* end_pos)
{
    const auto pos = Cool::String::find_matching_pair(source, '(', ')');
    if (!pos.has_value()) {
        throw std::invalid_argument("Couldn't parse the input declaration. It should look like \"( )\"");
    }
    *end_pos = pos->second + 1;
    return std::string{source.substr(pos->first, pos->second - pos->first + 1)};
}

std::string find_body(std::string_view source, size_t* end_pos)
{
    const auto pos = Cool::String::find_matching_pair(source, '{', '}');
    if (!pos.has_value()) {
        throw std::invalid_argument("Couldn't parse the body. It should be delimited by \"{ }\"");
    }
    *end_pos = pos->second + 1;
    return std::string{source.substr(pos->first, pos->second - pos->first + 1)};
}