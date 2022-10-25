#pragma once
#include <Cool/Nodes/Pin.h>

namespace Lab {

class Node {
public:
    auto input_pins() const -> const auto& { return _input_pins; }
    auto input_pins() -> auto& { return _input_pins; }
    auto output_pins() const -> const auto& { return _output_pins; }
    auto output_pins() -> auto& { return _output_pins; }

    std::string definition_name;

private:
    std::vector<Cool::Pin> _input_pins;
    std::vector<Cool::Pin> _output_pins;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
    }
};

} // namespace Lab