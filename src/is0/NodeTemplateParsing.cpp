#include "NodeTemplateParsing.h"
#include <Cool/String/String.h>

void parse_body(NodeTemplate& node_template, std::string_view source)
{
    node_template.sdf_identifiers = parse_capture(find_capture(source));
    // // std::regex r{R"(\[(([^\s]+)\s*)\])"};
    // // std::regex r{R"(\[(.+?)(?:,\s*|$)\])"};
    // std::regex r{R"(\[(.*)\].*?\((.*)\)?.*?\{(.*)\}(.*))"};

    // // flag type for determining the matching behavior (in this case on string objects)
    // std::smatch m;

    // // regex_search that searches pattern regexp in the string mystr
    // std::regex_search(source, m, r);

    // for (const auto x : m)
    //     Cool::Log::info(x.str());
}

auto find_capture(std::string_view source) -> std::string_view
{
    const auto capture_pos = Cool::String::find_matching_pair(source, '[', ']');
    if (!capture_pos.has_value()) {
        throw std::invalid_argument("Couldn't parse the capture group. It should de balimited by \"[ ]\"");
    }
    return source.substr(capture_pos->first + 1, capture_pos->second - capture_pos->first - 1);
}

auto parse_capture(std::string_view source) -> std::vector<std::string>
{
    return Cool::String::split_into_words(std::string{source});
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
    return max(${SDF1}(weird_pos), ${SDF2}(weird_pos));
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