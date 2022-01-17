#pragma once
#include <Cool/Parameter/Parsing.h>
#include <Cool/String/String.h>

std::string_view find_capture(std::string_view source, size_t* end_pos);

std::vector<std::string> parse_capture(std::string_view source);

std::string find_input_declaration(std::string_view source, size_t* end_pos);

std::string find_body(std::string_view source, size_t* end_pos);