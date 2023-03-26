#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Nodes/Pin.h>
#include <cereal/archives/json.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/polymorphic.hpp>
#include <optional>
#include <string_view>
#include "Cool/Nodes/BaseNode.h"
#include "Cool/Nodes/NodeDefinitionIdentifier.h"
#include "Module_Nodes/PrimitiveType.h"

namespace Lab {

class Node : public Cool::BaseNode {
public:
    Node() = default;
    Node(Cool::NodeDefinitionIdentifier const& id_names, size_t number_of_main_input_pins, size_t number_of_function_inputs, bool is_template_node)
        : _id_names{id_names}
        , _number_of_main_input_pins{number_of_main_input_pins}
        , _number_of_function_inputs{number_of_function_inputs}
        , _chosen_any_type{is_template_node ? std::make_optional(PrimitiveType::Float) : std::nullopt}
    {}

    auto name() const -> std::string { return _name; }
    void set_name(std::string const& name) { _name = name; }

    auto definition_name() const -> std::string override { return _id_names.definition_name; }
    auto category_name() const -> std::string override { return _id_names.category_name; }

    auto id_names() const -> Cool::NodeDefinitionIdentifier { return _id_names; }

    auto value_inputs() -> auto& { return _value_inputs; }
    auto value_inputs() const -> auto const& { return _value_inputs; }

    auto number_of_main_input_pins() const -> size_t { return _number_of_main_input_pins; }
    auto main_input_pin(size_t main_input_index) const -> Cool::InputPin const&
    {
        assert(main_input_index < number_of_main_input_pins());
        return input_pins()[main_input_index];
    }
    auto main_output_pin() const -> Cool::OutputPin const& { return output_pins()[0]; }
    /// This corresponds to a subset of all the input_pints(); the ones that correspond to an INPUT of the node.
    auto pin_of_function_input(size_t input_index) const -> Cool::InputPin const& { return input_pins()[_number_of_main_input_pins + input_index]; }
    auto pin_of_value_input(size_t property_index) const -> Cool::InputPin const& { return input_pins()[_number_of_main_input_pins + _number_of_function_inputs + property_index]; }
    auto pin_of_output_index(size_t output_index_index) const -> Cool::OutputPin const& { return output_pins()[_number_of_main_input_pins + output_index_index]; }

    /// Only call this if this node is a template node
    auto chosen_any_type() const -> PrimitiveType { return _chosen_any_type.value(); }
    auto imgui_chosen_any_type() -> bool;

private:
    Cool::NodeDefinitionIdentifier _id_names;
    std::string                    _name{};

    std::vector<Cool::AnyInput>  _value_inputs;
    size_t                       _number_of_main_input_pins{};
    size_t                       _number_of_function_inputs{};

    std::optional<PrimitiveType> _chosen_any_type{}; // Only present if the node has `Any` in its signature.

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(
            cereal::base_class<Cool::BaseNode>(this),
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("Definition", _id_names),
            cereal::make_nvp("Value inputs", _value_inputs),
            cereal::make_nvp("Number of main input pins", _number_of_main_input_pins),
            cereal::make_nvp("Number of function inputs", _number_of_function_inputs),
            cereal::make_nvp("Chosen Any type", _chosen_any_type)
        );
#else
        (void)archive;
#endif
    }
};

auto to_string(Node const& node) -> std::string;

} // namespace Lab

CEREAL_REGISTER_TYPE(Lab::Node); // NOLINT