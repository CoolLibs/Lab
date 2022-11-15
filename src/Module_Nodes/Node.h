#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Nodes/Pin.h>

namespace Lab {

class Node {
public:
    Node() = default;
    explicit Node(std::string_view definition_name)
        : _definition_name{definition_name}
    {}

    auto definition_name() const -> std::string { return _definition_name; }

    auto input_pins() const -> auto const& { return _input_pins; }
    auto input_pins() -> auto& { return _input_pins; }
    auto output_pins() const -> auto const& { return _output_pins; }
    auto output_pins() -> auto& { return _output_pins; }
    auto properties() -> auto& { return _properties; }
    auto properties() const -> auto const& { return _properties; }

private:
    std::string                  _definition_name;
    std::vector<Cool::InputPin>  _input_pins;
    std::vector<Cool::OutputPin> _output_pins;
    std::vector<Cool::AnyInput>  _properties;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Definition", _definition_name),
            cereal::make_nvp("Input Pins", _input_pins),
            cereal::make_nvp("Output Pins", _output_pins),
            cereal::make_nvp("Properties", _properties)
        );
    }
};

} // namespace Lab