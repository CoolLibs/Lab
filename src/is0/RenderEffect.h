#pragma once

#include <Cool/Parameter/Parameter.h>
#include <Cool/Parameter/ParameterList.h>

struct RenderEffect {
    std::string         name;
    std::string         parameters_declaration;
    std::string         code_template;
    Cool::ParameterList parameters;
    bool                is_active = true;
};