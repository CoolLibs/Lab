#include "RenderEffectParsing.h"
#include "ParsingFunction.h"

void parse_render_effect(RenderEffect& render_effect, std::string_view source)
{
    size_t     next_pos    = 0;
    const auto trim_source = [&]() {
        source = source.substr(next_pos, source.length() - next_pos);
    };
    render_effect.extra_code = find_extra_code(source, &next_pos);
    trim_source();
    render_effect.parameters_declaration = find_input_declaration(source, &next_pos);
    trim_source();
    render_effect.code_template = find_body(source, &next_pos);
    trim_source();
    const auto parameters    = Cool::parse_all_parameter_desc(source);
    render_effect.parameters = Cool::ParameterList::ParameterList(parameters);
}