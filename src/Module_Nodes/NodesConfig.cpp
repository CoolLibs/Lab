#include "NodesConfig.h"
#include <Cool/Dependencies/requires_shader_code_generation.h>
#include "Cool/ImGui/ImGuiExtras.h"
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

    auto* def = _get_node_definition(node.definition_name());
    if (!def)
    {
        Cool::ImGuiExtras::warning_text("Node definition file not found");
    }
    else if (node.properties().size() != 0)
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

auto NodesConfig::make_node(NodeDefinition const& def) const -> Node
{
    auto node = Node{def.name(), def.signature().arity, def.inputs().size(), def.signature().is_template()};

    for (size_t i = 0; i < def.signature().arity; ++i)
        node.input_pins().push_back(Cool::InputPin{fmt::format("IN{}", i + 1)});
    node.output_pins().push_back(Cool::OutputPin{"OUT"});

    for (auto const& input : def.inputs())
        node.input_pins().push_back(Cool::InputPin{input.name()});

    for (auto const& property_def : def.properties())
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
    def.presets_manager().apply_first_preset_if_there_is_one(settings);
    // Apply back the variables to the inputs' default variables
    apply_settings_to_inputs(settings, node.properties(), _ui.variable_registries());

    for (auto const& output_index_name : def.output_indices())
        node.output_pins().push_back(Cool::OutputPin{output_index_name});

    return node;
}

} // namespace Lab
