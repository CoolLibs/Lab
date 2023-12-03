#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Nodes/Pin.h>
#include <cereal/types/optional.hpp>
#include <optional>
#include <string_view>
#include "Cool/Nodes/NodeDefinitionIdentifier.h"
#include "Module_Nodes/PrimitiveType.h"

namespace Lab {

class Node {
public:
    Node() = default;
    Node(Cool::NodeDefinitionIdentifier const& id_names, size_t number_of_main_input_pins, size_t number_of_function_inputs)
        : _id_names{id_names}
        , _number_of_main_input_pins{number_of_main_input_pins}
        , _number_of_function_inputs{number_of_function_inputs}
    {}

    auto name() const -> std::string { return _name; }
    void set_name(std::string const& name) { _name = name; }

    auto definition_name() const -> std::string { return _id_names.definition_name; }
    auto category_name() const -> std::string { return _id_names.category_name; }

    auto id_names() const -> Cool::NodeDefinitionIdentifier { return _id_names; }

    [[nodiscard]] auto input_pins() const -> auto const& { return _input_pins; }
    [[nodiscard]] auto input_pins() -> auto& { return _input_pins; }
    [[nodiscard]] auto output_pins() const -> auto const& { return _output_pins; }
    [[nodiscard]] auto output_pins() -> auto& { return _output_pins; }
    [[nodiscard]] auto value_inputs() -> auto& { return _value_inputs; }
    [[nodiscard]] auto value_inputs() const -> auto const& { return _value_inputs; }

    auto number_of_main_input_pins() const -> size_t { return _number_of_main_input_pins; }
    auto main_input_pin(size_t main_input_index) const -> Cool::InputPin const&
    {
        assert(main_input_index < number_of_main_input_pins());
        return input_pins()[main_input_index];
    }
    auto main_output_pin() const -> Cool::OutputPin const& { return output_pins()[0]; }
    /// This corresponds to a subset of all the input_pints(); the ones that correspond to an INPUT of the node.
    auto pin_of_function_input(size_t function_input_index) const -> Cool::InputPin const& { return input_pins()[function_input_pin_idx_begin() + function_input_index]; }
    auto pin_of_value_input(size_t value_input_index) const -> Cool::InputPin const& { return input_pins()[value_input_pin_idx_begin() + value_input_index]; }
    auto pin_of_output_index(size_t output_index_index) const -> Cool::OutputPin const& { return output_pins()[1 + output_index_index]; }

    auto main_input_pin_idx_begin() const -> size_t { return 0; }
    auto main_input_pin_idx_end() const -> size_t { return _number_of_main_input_pins; }
    auto function_input_pin_idx_begin() const -> size_t { return main_input_pin_idx_end(); }
    auto function_input_pin_idx_end() const -> size_t { return function_input_pin_idx_begin() + _number_of_function_inputs; }
    auto value_input_pin_idx_begin() const -> size_t { return function_input_pin_idx_end(); }
    auto value_input_pin_idx_end() const -> size_t { return _input_pins.size(); }

    auto is_audio_node() const -> bool;

private:
    Cool::NodeDefinitionIdentifier _id_names;
    std::string                    _name{};

    std::vector<Cool::InputPin>  _input_pins{};
    std::vector<Cool::OutputPin> _output_pins{};
    std::vector<Cool::AnyInput>  _value_inputs;
    size_t                       _number_of_main_input_pins{};
    size_t                       _number_of_function_inputs{};

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("Definition", _id_names),
            cereal::make_nvp("Input Pins", _input_pins),
            cereal::make_nvp("Output Pins", _output_pins),
            cereal::make_nvp("Value inputs", _value_inputs),
            cereal::make_nvp("Number of main input pins", _number_of_main_input_pins),
            cereal::make_nvp("Number of function inputs", _number_of_function_inputs)
        );
    }
};

auto to_string(Node const& node) -> std::string;

} // namespace Lab
