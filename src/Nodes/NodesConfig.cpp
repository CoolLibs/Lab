#include "NodesConfig.h"
#include <Cool/Dependencies/always_requires_shader_code_generation.h>
#include <algorithm>
#include <string>
#include "Cool/Audio/AudioManager.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Nodes/Pin.h"
#include "Cool/Nodes/utilities/drawing.h"
#include "Cool/String/String.h"
#include "Cool/StrongTypes/Color.h"
#include "FunctionSignature.h"
#include "Node.h"
#include "NodeColor.h"
#include "NodeDefinition.h"
#include "PrimitiveType.h"
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
            const auto maybe_variable = registry.get(input._default_variable_id.raw());
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
    Cool::VariableRegistries&             registry,
    std::string_view                      node_name
)
{
    try
    {
        for (size_t i = 0; i < inputs.size(); ++i)
        {
            std::visit([&](auto&& input) {
                registry.set(
                    input._default_variable_id.raw(),
                    get_concrete_variable(input, settings.at(i))
                );
            },
                       inputs.at(i));
        }
    }
    catch (...)
    {
        // TODO(JF) Remove this try-catch once we update presets properly
        Cool::Log::ToUser::warning(
            "Presets",
            fmt::format(
                "Current preset for node \"{}\" does not match the INPUTs of the shader anymore, it has not been applied fully.",
                node_name
            )
        );
    }
}

auto NodesConfig::name(Cool::Node const& abstract_node) const -> std::string
{
    auto const& node = abstract_node.downcast<Node>();
    return node.name();
}

auto NodesConfig::category_name(Cool::Node const& abstract_node) const -> std::string
{
    auto const& node = abstract_node.downcast<Node>();
    return node.category_name();
}

void NodesConfig::main_node_toggle(Cool::NodeId const& node_id)
{
    bool const was_main = node_id == _main_node_id;
    bool       is_main  = was_main;
    if (Cool::ImGuiExtras::toggle("Main node", &is_main))
    {
        if (is_main)
            set_main_node_id(node_id);
        else
            set_main_node_id({}); // Unselect main node
    }
}

void NodesConfig::imgui_above_node_pins(Cool::Node& /* abstract_node */, Cool::NodeId const& id)
{
    main_node_toggle(id);
}

void NodesConfig::imgui_below_node_pins(Cool::Node& /* abstract_node */, Cool::NodeId const& /* id */)
{
}

static auto value_input_is_connected_to_a_node(size_t value_input_index, Node const& node, Cool::NodesGraph const& graph) -> bool
{
    auto const  input_pin     = node.pin_of_value_input(value_input_index); // NOLINT(performance-unnecessary-copy-initialization)
    auto const  input_node_id = graph.find_node_connected_to_input_pin(input_pin.id());
    auto const* input_node    = graph.try_get_node<Node>(input_node_id);
    return input_node != nullptr;
}

void NodesConfig::imgui_in_inspector_above_node_info(Cool::Node& /* abstract_node */, Cool::NodeId const& id)
{
    // auto& node = abstract_node.downcast<Node>();

    main_node_toggle(id);
}

void NodesConfig::imgui_in_inspector_below_node_info(Cool::Node& abstract_node, Cool::NodeId const& /* id */)
{
    auto& node = abstract_node.downcast<Node>();

    ImGui::NewLine();

    if (node.is_audio_node())
    {
        if (Cool::ImGuiExtras::button_with_text_icon(ICOMOON_COG))
            _audio_manager.open_imgui_window();
        ImGui::SetItemTooltip("%s", "Open audio config to select which audio file to play, or which input device (e.g. microphone) to use.");
    }

    if (node.is_particle_initializer())
    {
        if (node.particles_count().has_value())
            ImGui::DragScalar("Particles Count", ImGuiDataType_U64, &node.particles_count().value());
        else
        {
            assert(false);
        }
    }

    for (size_t i = 0; i < node.value_inputs().size(); ++i)
    {
        Cool::ImGuiExtras::disabled_if(
            value_input_is_connected_to_a_node(i, node, _graph),
            "This value is connected to a node in the graph, you cannot edit it here.", [&]() {
                _ui.widget(node.value_inputs()[i]);
            }
        );
    }

    auto* def = _get_mutable_node_definition(node.id_names());
    if (!def)
    {
        ImGui::NewLine();
        ImGui::Separator();
        Cool::ImGuiExtras::warning_text("Node definition file not found");
    }
    else if (!node.value_inputs().empty())
    {
        ImGui::NewLine();
        // Get the variables from the inputs
        auto settings = settings_from_inputs(node.value_inputs(), _ui.variable_registries());
        // Apply
        bool const has_changed = def->imgui_presets(settings);
        // Apply back the variables to the inputs' default variables
        apply_settings_to_inputs(settings, node.value_inputs(), _ui.variable_registries(), to_string(node));

        if (has_changed)
            _ui.set_dirty(_regenerate_code_flag); // TODO(JF) We could simply rerender instead of regenerate if none of the properties require code generation
    }
}

auto NodesConfig::node_color(Cool::Node const& abstract_node, Cool::NodeId const&) const -> Cool::Color
{
    auto const& node = abstract_node.downcast<Node>();
    auto const* def  = _get_node_definition(node.id_names());
    if (!def)
        return Cool::Color::from_srgb(glm::vec3{0.f});

    return compute_function_color(def->signature());
}

auto NodesConfig::pin_color(Cool::Pin const& pin, size_t pin_index, Cool::Node const& abstract_node, Cool::NodeId const& node_id) const -> Cool::Color
{
    if (pin.kind() == Cool::PinKind::Output)
    {
        // Main output pin
        if (pin_index == 0)
            return node_color(abstract_node, node_id);
        // Float output
        return NodeColor::greyscale();
    }

    auto const& node = abstract_node.downcast<Node>();
    auto const* def  = _get_node_definition(node.id_names());
    if (!def)
        return Cool::Color::from_srgb(glm::vec3{0.f});

    // For function inputs, use the color of the function's signature.
    if (node.function_input_pin_idx_begin() <= pin_index && pin_index < node.function_input_pin_idx_end())
    {
        return compute_function_color(def->function_inputs()[pin_index - node.function_input_pin_idx_begin()].signature());
    }
    // For value inputs, use the color corresponding to the Input type.
    if (node.value_input_pin_idx_begin() <= pin_index && pin_index < node.value_input_pin_idx_end())
    {
        return compute_value_input_color(def->value_inputs()[pin_index - node.value_input_pin_idx_begin()]);
    }
    // For main input pins, use the color corresponding to the `from` Primitive type.
    if (node.main_input_pin_idx_begin() <= pin_index && pin_index < node.main_input_pin_idx_end())
    {
        return compute_primitive_type_color(def->signature().from);
    }

    return NodeColor::miscellaneous();
}

void NodesConfig::on_node_created(Cool::Node& /* abstract_node */, Cool::NodeId const& node_id, Cool::Pin const* pin_linked_to_new_node)
{
    _ui.set_dirty(_regenerate_code_flag);
    _node_we_might_want_to_restore_as_main_node_id = {};

    // Don't change main node if we are dragging a link backward.
    if (pin_linked_to_new_node && pin_linked_to_new_node->kind() == Cool::PinKind::Input)
        return;

    if (!pin_linked_to_new_node)
        _node_we_might_want_to_restore_as_main_node_id = _main_node_id; // Keep track, so that if someone has a main node, creates a separate node, and then plugs that node back into the old main node, we will restore it as the main node, even if it itself is plugged into a node.
    set_main_node_id(node_id, true /*keep_node_we_might_want_to_restore_as_main_node*/);
}

void NodesConfig::set_main_node_id(Cool::NodeId const& id, bool keep_node_we_might_want_to_restore_as_main_node)
{
    if (!keep_node_we_might_want_to_restore_as_main_node)
        _node_we_might_want_to_restore_as_main_node_id = {};
    _main_node_id = id;
    _ui.set_dirty(_regenerate_code_flag);
}

void NodesConfig::on_link_created_between_existing_nodes(Cool::Link const& link, Cool::LinkId const&)
{
    // Traverse the graph until:
    // - We reach the end and set this as the main node
    // - We reach the main node and do nothing
    // - We reach the previous main_node and set this as the main node

    auto next_id = _graph.find_node_containing_pin(link.to_pin_id);
    if (next_id == _main_node_id)
        return;
    if (next_id == _node_we_might_want_to_restore_as_main_node_id && !_node_we_might_want_to_restore_as_main_node_id.underlying_uuid().is_nil())
    {
        set_main_node_id(next_id);
        return;
    }
    auto const* next_node        = _graph.try_get_node<Node>(next_id);
    auto        new_main_node_id = Cool::NodeId{};
    while (next_node)
    {
        new_main_node_id = next_id;
        next_id          = _graph.find_node_connected_to_output_pin(next_node->output_pins()[0].id());
        if (next_id == _main_node_id)
            return;
        if (next_id == _node_we_might_want_to_restore_as_main_node_id && !_node_we_might_want_to_restore_as_main_node_id.underlying_uuid().is_nil())
        {
            set_main_node_id(next_id);
            return;
        }
        next_node = _graph.try_get_node<Node>(next_id);
    }
    if (!new_main_node_id.underlying_uuid().is_nil())
        set_main_node_id(new_main_node_id);
}

static auto doesnt_need_main_pin(FunctionSignature const& signature) -> bool
{
    return
        // Image-like and shape 2D
        (signature.from == PrimitiveType::UV && signature.to != PrimitiveType::UV)
        // Curve
        || is_curve(signature)
        // Shape 3D
        || is_shape_3D(signature)
        // Particle nodes that are not Particle Modifiers
        || (signature.from == PrimitiveType::Particle2D && signature.to != PrimitiveType::Particle2D)
        || (signature.from == PrimitiveType::Particle3D && signature.to != PrimitiveType::Particle3D);
}

auto NodesConfig::make_node(Cool::NodeDefinitionAndCategoryName const& cat_id) -> Node
{
    auto const def = cat_id.def.downcast<NodeDefinition>();

    bool const needs_main_pin = !doesnt_need_main_pin(def.signature());

    auto node = Node{
        {def.name(), cat_id.category_name},
        needs_main_pin ? def.signature().arity : 0,
        def.function_inputs().size(),
    };
    auto const node_category_config = _get_node_category_config(cat_id.category_name);

    if (is_particle_initializer(def.signature()))
    {
        node.particles_count() = std::make_optional<size_t>(1000);
    }

    if (needs_main_pin)
    {
        for (size_t i = 0; i < def.signature().arity; ++i)
        {
            node.input_pins().push_back(Cool::InputPin{/*pin_name=*/Cool::String::replace_all(def.main_parameter_names()[i], "_", " ")});
        }
    }
    node.output_pins().emplace_back("OUT");

    for (auto const& function_input : def.function_inputs())
        node.input_pins().push_back(Cool::InputPin{function_input.name()});

    for (auto const& value_input_def : def.value_inputs())
    {
        node.value_inputs().push_back(_input_factory.make(
            value_input_def,
            Cool::always_requires_shader_code_generation(value_input_def) ? _regenerate_code_flag : _rerender_flag,
            _regenerate_code_flag // At the moment only used by Gradient variable when we detect that the number of marks has changed. See `set_value_default_impl()` of Command_SetVariable.h
        ));
        node.input_pins().push_back(Cool::InputPin{std::visit([](auto&& value_input_def) { return value_input_def.name; }, value_input_def)});
    }

    // Get the variables from the inputs
    auto settings = settings_from_inputs(node.value_inputs(), _ui.variable_registries());
    // Apply
    def.presets_manager().apply_first_preset_if_there_is_one(settings);
    // Apply back the variables to the inputs' default variables
    apply_settings_to_inputs(settings, node.value_inputs(), _ui.variable_registries(), to_string(node));

    for (auto const& output_index_name : def.output_indices())
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
    return std::find_if(old_inputs.begin(), old_inputs.end(), [&](const Cool::AnyInput& other_input) {
        return inputs_have_the_same_type_and_name(other_input, input);
    });
}

static void keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(
    std::vector<Cool::AnyInput>& old_inputs,
    std::vector<Cool::AnyInput>& new_inputs
)
{
    for (auto& input : old_inputs)
    {
        auto const it = iterator_to_same_input(input, new_inputs);
        if (it != new_inputs.end())
        {
            auto       description         = std::visit([](auto&& input) { return std::move(input._description); }, *it); // Keep the new description
            auto const desired_color_space = std::visit([](auto&& input) { return input._desired_color_space; }, *it);    // Keep the new desired_color_space
            *it                            = std::move(input);
            std::visit([&](auto&& it) mutable { it._description = std::move(description); }, *it);
            std::visit([&](auto&& it) { it._desired_color_space = desired_color_space; }, *it);
        }
    }
}

template<typename PinT>
static void refresh_pins(std::vector<PinT>& new_pins, std::vector<PinT> const& old_pins, std::function<void(Cool::PinId const&)> remove_links)
{
    // Collect indices of pins that don't match any old pins.
    // Start with all the indices of the new input pins
    std::vector<size_t> indices(new_pins.size());
    std::iota(std::begin(indices), std::end(indices), 0u);
    // Remove indices of pins that have the same name as an old pin.
    std::erase_if(indices, [&](size_t idx) {
        return std::any_of(old_pins.begin(), old_pins.end(), [&](auto&& pin) {
            return pin.name() == new_pins[idx].name();
        });
    });
    // Make sure smallest indices are last because we read this vector from right to left.
    std::sort(indices.begin(), indices.end(), std::greater{});

    for (auto const& pin : old_pins)
    {
        // Try to find a pin with the same name
        auto const it = std::find_if(new_pins.begin(), new_pins.end(), [&](auto&& other_pin) {
            return other_pin.name() == pin.name();
        });
        if (it != new_pins.end())
        {
            it->set_id(pin.id());
        }
        // Otherwise default to one of the leftover indices
        else if (!indices.empty())
        {
            new_pins[indices.back()].set_id(pin.id());
            indices.pop_back();
        }
        else
        {
            remove_links(pin.id());
        }
    }
}

void NodesConfig::update_node_with_new_definition(Cool::Node& abstract_out_node, Cool::NodeDefinition const& definition, Cool::NodesGraph& graph)
{
    auto& out_node = abstract_out_node.downcast<Node>();
    auto  node     = make_node({definition, out_node.category_name()});

    node.set_name(out_node.name());
    node.set_particles_count(out_node.particles_count());

    keep_values_of_inputs_that_already_existed_and_destroy_unused_ones(out_node.value_inputs(), node.value_inputs());

    refresh_pins(node.input_pins(), out_node.input_pins(), [&](Cool::PinId const& pin_id) { graph.remove_link_going_into(pin_id); });
    refresh_pins(node.output_pins(), out_node.output_pins(), [&](Cool::PinId const& pin_id) { graph.remove_link_coming_from(pin_id); });

    out_node = std::move(node);
}

void NodesConfig::widget_to_rename_node(Cool::Node& abstract_node)
{
    auto& node = abstract_node.downcast<Node>();
    auto  name = node.name();
    ImGui::PushID(21654);
    ImGui::InputText("Title", &name);
    ImGui::PopID();
    node.set_name(name);
}

auto NodesConfig::maybe_disable_node_definition() const -> Cool::MaybeDisableNodeDefinition
{
#if !defined(COOL_PARTICLES_DISABLED_REASON)
    return [](Cool::NodeDefinition const&, Cool::NodesCategory const&) -> std::optional<const char*> {
        return std::nullopt;
    };
#else
    return [](Cool::NodeDefinition const&, Cool::NodesCategory const& category) -> std::optional<const char*> {
        if (Cool::String::contains(category.name(), "Particle"))
            return COOL_PARTICLES_DISABLED_REASON;
        return std::nullopt;
    };
#endif
}

} // namespace Lab
