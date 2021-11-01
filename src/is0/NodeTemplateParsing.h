#pragma once

#include "NodeTemplate.h"

void parse_body(NodeTemplate& node_template, std::string_view source);

auto find_capture(std::string_view source) -> std::string_view;
auto parse_capture(std::string_view source) -> std::vector<std::string>;
