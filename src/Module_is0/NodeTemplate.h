#pragma once

#include <Cool/Parameter/Parameter.h>

struct NodeTemplate {
    std::string                           name;
    std::string                           extra_code;
    std::string                           vec3_input_declaration;
    std::string                           code_template;
    std::vector<Cool::Parameter::AnyDesc> parameters;
    std::vector<std::string>              sdf_identifiers;
};