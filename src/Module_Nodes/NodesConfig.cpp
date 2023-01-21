#include "NodesConfig.h"
#include <Cool/Dependencies/requires_shader_code_generation.h>
#include <string>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Module_Nodes/FunctionSignature.h"
#include "Module_Nodes/Node.h"
#include "Module_Nodes/NodeDefinition.h"
#include "Module_Nodes/PrimitiveType.h"
#include "imgui.h"

namespace Lab {

// TODO(JF) Remove
static auto settings_from_inputs(
    const std::vector<Cool::AnyInput>& inputs,
    const Cool::VariableRegistries&    registry
) -> std::vector<Cool::AnyVariable>
{
    std::vector<Cool::AnyVariable> settings;
    settings.reserve(inputs.size());
    for (const auto& input : inputs)
    {
        const auto maybe_variable = std::visit([&](auto&& input) -> std::optional<Cool::AnyVariable> {
            const auto maybe_variable = registry.get(input._default_variable_id);
            if (maybe_variable)
            {
                return Cool::AnyVariable{*maybe_variable};
            }
            else
            {
                return std::nullopt;
            }
        },
                                               input);
        if (maybe_variable)
        {
            settings.push_back(*maybe_variable);
        }
    }
    return settings;
}

// TODO(JF) Remove
template<typename T>
auto get_concrete_variable(const Cool::Input<T>&, const Cool::AnyVariable& var) -> Cool::Variable<T>
{
    return std::get<Cool::Variable<T>>(var);
}

// TODO(JF) Remove
static void apply_settings_to_inputs(
    const std::vector<Cool::AnyVariable>& settings,
    std::vector<Cool::AnyInput>&          inputs,
    Cool::VariableRegistries&             registry
)
{
    try
    {
        for (size_t i = 0; i < inputs.size(); ++i)
        {
            std::visit([&](auto&& input) {
                registry.set(
                    input._default_variable_id,
                    get_concrete_variable(input, settings.at(i))
                );
            },
                       inputs.at(i));
        }
    }
    catch (...)
    {
        // TODO(JF) Remove this try-catch once we update presets properly
        Cool::Log::ToUser::warning("Presets", "This preset does not match the INPUTs of the shader anymore, it has not been applied fully.");
    }
}

auto NodesConfig::name(Node const& node) const -> std::string
{
    return node.definition_name();
}

auto NodesConfig::category_name(Node const& node) const -> std::string
{
    return node.category_name();
}

void NodesConfig::imgui_node_body(Node& node, Cool::NodeId const& id) const
{
    { // Main node selector
        const bool was_main = id == _main_node_id;
        bool       is_main  = was_main;
        if (ImGui::Checkbox("Main node", &is_main))
        {
            if (is_main && !was_main)
            {
                _main_node_id = id;
                _ui.set_dirty(_regenerate_code_flag);
            }
        }
    }

    if (node.imgui_chosen_any_type())
        _ui.set_dirty(_regenerate_code_flag);

    for (auto& property : node.properties())
        _ui.widget(property);

    auto* def = _get_node_definition(node.id_names());
    if (!def)
    {
        ImGui::NewLine();
        ImGui::Separator();
        Cool::ImGuiExtras::warning_text("Node definition file not found");
    }
    else if (!node.properties().empty())
    {
        ImGui::NewLine();
        ImGui::Separator();
        // Get the variables from the inputs
        auto settings = settings_from_inputs(node.properties(), _ui.variable_registries());
        // Apply
        bool const has_changed = def->imgui_presets(settings);
        // Apply back the variables to the inputs' default variables
        apply_settings_to_inputs(settings, node.properties(), _ui.variable_registries());

        if (has_changed)
            _ui.set_dirty(_regenerate_code_flag); // TODO(JF) We could simply rerender instead of regenerate if none of the properties require code generation
    }
}

static auto doesnt_need_main_pin(FunctionSignature const& signature) -> bool
{
    return signature.from == PrimitiveType::UV && signature.to != PrimitiveType::UV;
}

auto NodesConfig::make_node(Cool::NodeDefinitionAndCategoryName<NodeDefinition> const& cat_id) const -> Node
{
    bool const needs_main_pin = !doesnt_need_main_pin(cat_id.def.signature());

    auto node = Node{
        {cat_id.def.name(), cat_id.category_name},
        needs_main_pin ? cat_id.def.signature().arity : 0,
        cat_id.def.inputs().size(),
        cat_id.def.signature().is_template(),
    };

    if (needs_main_pin)
    {
        for (size_t i = 0; i < cat_id.def.signature().arity; ++i)
            node.input_pins().emplace_back(cat_id.def.main_parameter_names()[i]);
    }
    node.output_pins().emplace_back("");

    for (auto const& input : cat_id.def.inputs())
        node.input_pins().push_back(Cool::InputPin{input.name()});

    for (auto const& property_def : cat_id.def.properties())
    {
        node.properties().push_back(_input_factory.make(
            property_def,
            Cool::requires_shader_code_generation(property_def) ? _regenerate_code_flag : _rerender_flag
        ));
        node.input_pins().push_back(Cool::InputPin{std::visit([](auto&& property_def) { return property_def.name; }, property_def)});
    }

    // Get the variables from the inputs
    auto settings = settings_from_inputs(node.properties(), _ui.variable_registries());
    // Apply
    cat_id.def.presets_manager().apply_first_preset_if_there_is_one(settings);
    // Apply back the variables to the inputs' default variables
    apply_settings_to_inputs(settings, node.properties(), _ui.variable_registries());

    for (auto const& output_index_name : cat_id.def.output_indices())
        node.output_pins().push_back(Cool::OutputPin{output_index_name});

    return node;
}

static auto name(Cool::AnyInput const& input)
{
    return std::visit(([](auto&& input) { return input.name(); }), input);
}

static auto inputs_have_the_same_type_and_name(Cool::AnyInput const& input1, Cool::AnyInput const& input2) -> bool
{
    return input1.index() == input2.index()
           && name(input1) == name(input2);
}

static auto iterator_to_same_input(Cool::AnyInput const& input, std::vector<Cool::AnyInput>& old_inputs)
{
    return std::ranges::find_if(old_inputs, [&](const Cool::AnyInput& other_input) {
        return inputs_have_the_same_type_and_name(other_input, input);
    });
}

static void keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(
    std::vector<Cool::AnyInput>& old_inputs,
    std::vector<Cool::AnyInput>& new_inputs
    // Cool::InputDestructor_Ref    destroy
)
{
    for (auto& input : old_inputs)
    {
        auto const it = iterator_to_same_input(input, new_inputs);
        if (it != new_inputs.end())
        {
            auto description = std::visit([](auto&& input) { return std::move(input._description); }, *it); // Keep the new description
            // destroy(*it);
            *it = std::move(input);
            std::visit([&](auto&& it) mutable { it._description = std::move(description); }, *it);
        }
        else
        {
            // destroy(input);
        }
    }
}

void NodesConfig::update_node_with_new_definition(Node& out_node, NodeDefinition const& definition, Cool::Graph<Node>& graph) const
{
    auto node = make_node({definition, out_node.category_name()});

    keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(out_node.properties(), node.properties());

    node.output_pins()[0].set_id(out_node.output_pins()[0].id());

    for (size_t i = 0; i < out_node.input_pins().size(); ++i)
    {
        auto const& pin = out_node.input_pins()[i];
        if (i < node.input_pins().size())
            node.input_pins()[i].set_id(pin.id());
        else
            graph.remove_link_going_into(pin.id());
    }

    out_node = node;
}

} // namespace Lab
