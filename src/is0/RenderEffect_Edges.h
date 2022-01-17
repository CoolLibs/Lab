#pragma once

#include <Cool/Parameter/Parameter.h>

struct RenderEffect_Edges {
    Cool::Parameter::Float size{{.name          = "Size",
                                 .default_value = 0.1,
                                 .min_value     = 0.001,
                                 .max_value     = 2.0}};
    Cool::Parameter::Color color;
    bool                   is_active = true;
};

std::string code_gen_edges_parameter(const RenderEffect_Edges& edges);
std::string code_gen_edges();

bool edges_imgui(RenderEffect_Edges& edges);