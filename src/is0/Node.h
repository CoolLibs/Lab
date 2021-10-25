#pragma once

#include <Cool/Parameter/ParameterList.h>
#include <Cool/Uuid/Uuid.h>

struct Node {
    std::string         node_template_name;
    Cool::ParameterList parameter_list{};
    Cool::Uuid          uuid{};
};
