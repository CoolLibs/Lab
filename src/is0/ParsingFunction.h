#pragma once
#include <Cool/Parameter/Parsing.h>
#include <Cool/String/String.h>

std::string find_extra_code(std::string_view source, size_t* end_pos);

std::string find_parameters_declaration(std::string_view source, size_t* end_pos);

std::string find_body(std::string_view source, size_t* end_pos);

TEST_CASE("[is0::ParsingFunction] find_extra_code()")
{
    // Given
    const auto source  = R"(
`
#include "something.glsl"
`
    )";
    size_t     end_pos = 0;
    // Then
    CHECK(find_extra_code(source, &end_pos) == R"(
#include "something.glsl"
)");
}
