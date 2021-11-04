#pragma once

#include <Cool/Parameter/ParameterList.h>
#include "Pin.h"

struct Node {
    NodeId                id{};
    std::string           node_template_name;
    Cool::ParameterList   parameter_list{};
    std::vector<PinSdfIn> input_pins;
    PinSdfOut             output_pin;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Id", id),
                cereal::make_nvp("Node Template", node_template_name),
                cereal::make_nvp("Parameters", parameter_list),
                cereal::make_nvp("Input pins", input_pins),
                cereal::make_nvp("Output pin", output_pin));
    }
};
