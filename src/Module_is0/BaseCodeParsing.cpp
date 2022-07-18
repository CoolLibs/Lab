#include "BaseCodeParsing.h"
#include <doctest/doctest.h>
#include "ParsingFunction.h"

void parse_base_code(BaseCode& base_code, std::string_view source)
{
    size_t     next_pos    = 0;
    const auto trim_source = [&]() {
        source = source.substr(next_pos, source.length() - next_pos);
    };
    base_code.extra_code = find_extra_code(source, &next_pos);
    trim_source();
    base_code.parameters_declaration = find_parameters_declaration(source, &next_pos);
    trim_source();
    base_code.code = find_body(source, &next_pos);
    trim_source();
    base_code.parameters = Cool::ParameterList(Cool::parse_all_parameter_desc(source));
}