#pragma once

#include <Cool/Parameter/Parameter.h>

struct NodeTemplate {
    std::string                           name;
    std::string                           code_template;
    std::vector<Cool::Parameter::AnyDesc> parameters;
    std::vector<std::string>              sdf_identifiers;
};