#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Nodes/Pin.h>

namespace Lab {

class Node {
public:
    Node() = default;
    Node(std::string_view definition_name, size_t number_of_inputs)
        : _definition_name{definition_name}
        , _number_of_inputs{number_of_inputs}
    {}

    auto definition_name() const -> std::string { return _definition_name; }

    auto input_pins() const -> auto const& { return _input_pins; }
    auto input_pins() -> auto& { return _input_pins; }
    auto output_pins() const -> auto const& { return _output_pins; }
    auto output_pins() -> auto& { return _output_pins; }
    auto properties() -> auto& { return _properties; }
    auto properties() const -> auto const& { return _properties; }

    auto main_input_pin() const -> Cool::InputPin const& { return _input_pins[0]; }
    auto main_output_pin() const -> Cool::OutputPin const& { return _output_pins[0]; }
    /// This corresponds to a subset of all the input_pints(); the ones that correspond to an INPUT of the node.
    auto pin_of_input(size_t input_index) const -> Cool::InputPin const& { return _input_pins[1 + input_index]; }
    auto pin_of_property(size_t property_index) const -> Cool::InputPin const& { return _input_pins[1 + _number_of_inputs + property_index]; }
    auto pin_of_output_index(size_t output_index_index) const -> Cool::OutputPin const& { return _output_pins[1 + output_index_index]; }

private:
    std::string                  _definition_name;
    std::vector<Cool::InputPin>  _input_pins;
    std::vector<Cool::OutputPin> _output_pins;
    std::vector<Cool::AnyInput>  _properties;
    size_t                       _number_of_inputs;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Definition", _definition_name),
            cereal::make_nvp("Input Pins", _input_pins),
            cereal::make_nvp("Output Pins", _output_pins),
            cereal::make_nvp("Properties", _properties),
            cereal::make_nvp("Number of inputs", _number_of_inputs)
        );
    }
};

} // namespace Lab