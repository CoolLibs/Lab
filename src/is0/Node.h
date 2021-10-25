#pragma once

#include <Cool/Parameter/ParameterList.h>
#include <Cool/Uuid/Uuid.h>
#include "Pin.h"

struct Node {
    Cool::Uuid            uuid{};
    std::string           node_template_name;
    Cool::ParameterList   parameter_list{};
    std::vector<PinSdfIn> input_pins;
    PinSdfOut             output_pin;
};
