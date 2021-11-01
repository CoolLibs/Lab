#include "NodeTemplateParsing.h"
#include <Cool/String/String.h>

static std::string_view find_capture(std::string_view source)
{
    const auto capture_pos = Cool::String::find_matching_pair(source, '[', ']');
    if (!capture_pos.has_value()) {
        throw std::invalid_argument("Couldn't parse the capture group. It should be delimited by \"[ ]\"");
    }
    return source.substr(capture_pos->first + 1, capture_pos->second - capture_pos->first - 1);
}

static std::vector<std::string> parse_capture(std::string_view source)
{
    return Cool::String::split_into_words(std::string{source});
}

static std::string find_sdf_input_declaration(std::string_view source)
{
    const auto pos = Cool::String::find_matching_pair(source, '(', ')');
    if (!pos.has_value()) {
        throw std::invalid_argument("Couldn't parse the sdf input declaration. It should look like \"(vec3 pos)\"");
    }
    return std::string{source.substr(pos->first, pos->second - pos->first + 1)};
}

static std::string find_sdf_body(std::string_view source)
{
    const auto pos = Cool::String::find_matching_pair(source, '{', '}');
    if (!pos.has_value()) {
        throw std::invalid_argument("Couldn't parse the sdf body. It should be delimited by \"{ }\"");
    }
    return std::string{source.substr(pos->first, pos->second - pos->first + 1)};
}

void parse_body(NodeTemplate& node_template, std::string_view source)
{
    node_template.sdf_identifiers        = parse_capture(find_capture(source));
    node_template.vec3_input_declaration = find_sdf_input_declaration(source);
    node_template.code_template          = find_sdf_body(source);
}

TEST_CASE("[is0::NodeParsing] parse_body()")
{
    // Given
    const std::string source = R"(
[MY_SDF1 MY_SDF2] (vec3 weird_pos) {
    return max(${MY_SDF1}(weird_pos), ${MY_SDF2}(weird_pos));
}
)";
    // When
    NodeTemplate node_template;
    parse_body(node_template, source);
    // Then
    CHECK(node_template.vec3_input_declaration == "(vec3 weird_pos)");
    CHECK(node_template.code_template == R"({
    return max(${MY_SDF1}(weird_pos), ${MY_SDF2}(weird_pos));
})");
    const std::vector<std::string> expected_sdf_identifiers = {"MY_SDF1", "MY_SDF2"};
    CHECK(std::ranges::is_permutation(node_template.sdf_identifiers, expected_sdf_identifiers));
}

TEST_CASE("[is0::NodeParsing] find_capture()")
{
    const std::string source = R"(
[MY_SDF1 MY_SDF2] (vec3 weird_pos) {
    return max(${MY_SDF1}(weird_pos), ${MY_SDF2}(weird_pos));
}
)";
    CHECK(find_capture(source) == "MY_SDF1 MY_SDF2");
}