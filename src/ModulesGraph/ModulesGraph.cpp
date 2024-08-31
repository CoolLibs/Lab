#include "ModulesGraph.h"
#include <CommandCore/CommandExecutor.h>
#include <Module_Particles/Module_Particles.h>
#include <Nodes/FunctionSignature.h>
#include <imgui.h>
#include <algorithm>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/Gpu/RenderTarget.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Module_Compositing/Module_Compositing.h"
#include "Module_Compositing/generate_compositing_shader_code.h"
#include "Module_FeedbackLoop/Module_FeedbackLoop.hpp"
#include "Module_Particles/Module_Particles.h"
#include "Module_Particles/generate_simulation_shader_code.h"
#include "ModulesGraph/ModulesGraph.h"
#include "Nodes/valid_glsl.h"
#include "UI/imgui_show.h"

namespace Lab {

void ModulesGraph::update()
{
    for (auto& module_node : _module_nodes)
    {
        module_node->module->_nodes_graph = &_nodes_editor.graph();
        module_node->module->update();
    }
}

void ModulesGraph::render(DataToPassToShader const& data_to_pass_to_shader, DataToGenerateShaderCode const& data_to_generate_shader_code)
{
    // TODO(FeedbackLoop) reintroduce it
    // if (_compositing_module.depends_on().time_since_last_midi_button_pressed
    //     || std::any_of(_particles_module_nodes.begin(), _particles_module_nodes.end(), [&](auto const& module_node) { return module_node->module.depends_on().time_since_last_midi_button_pressed; }))
    // {
    //     request_rerender_all(); // TODO(Modules) Only rerender the modules that depend on this
    // }

    // if (render_target.needs_resizing()) // TODO(FeedbackLoop) handle rerender when size changes
    //     request_rerender_all();
    if (rerender_all_flag().is_dirty())
    {
        request_rerender_all();
        rerender_all_flag().set_clean();
    }

    if (regenerate_code_flag().is_dirty())
    {
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::ToUser::info("Modules Graph", "Compiled");
        recreate_all_modules(_main_node_id, data_to_generate_shader_code);
        request_rerender_all();
        regenerate_code_flag().set_clean();
    }

    // TODO(FeedbackLoop) Is this still necessary ?
    // for (auto& module_node : _particles_module_nodes)
    //     module_node->render_target.set_size(render_target.desired_size());
    // _compositing_module.set_render_target_size(render_target.desired_size()); // Must be done before rendering, otherwise we might read a target that is too small. (e.g. 1 pixel on app startup)

    // TODO(Particles) Remove those _nodes_graph
    for (auto& module_node : _module_nodes)
    {
        module_node->module->_nodes_graph = &_nodes_editor.graph();
        // TODO(FeedbackLoop) Handle the fact that a node needs to rerender if one of its dependencies needs to rerender
        // if (module_node->module->needs_to_rerender())
        //     _compositing_module.needs_to_rerender_flag().set_dirty(); // Because compositing module depends on particles module
    }
    // TODO(Modules) TODO(FeedbackLoop) Render in the order of dependency between the modules
    for (auto& node : _module_nodes)
        render_one_module(*node, data_to_pass_to_shader);
}

void ModulesGraph::render_one_module(ModulesGraphNode& node, DataToPassToShader const& data)
{
    if (!node.module->needs_to_rerender())
        return;

    // Cool::Log::Debug::info("bob", fmt::format("{} x {}", data.system_values.render_target_size.width(), data.system_values.render_target_size.height()));
    node.module->do_rendering(data, node.dependencies);
    node.module->needs_to_rerender_flag().set_clean();

    if (DebugOptions::log_when_rendering())
        Cool::Log::ToUser::info(node.module->name() + " Module", "Rendered");
}

void ModulesGraph::request_rerender_all()
{
    for (auto& node : _module_nodes)
        node->module->needs_to_rerender_flag().set_dirty();
}

void ModulesGraph::on_time_reset()
{
    for (auto& node : _module_nodes)
        node->module->on_time_reset();
}

static auto texture_name_for_module(Cool::NodeId const& id) -> std::string
{
    using fmt::literals::operator""_a;
    return valid_glsl(fmt::format(
        FMT_COMPILE(
            R"STR(texture_{id})STR"
        ),
        "id"_a = to_string(id.underlying_uuid())
    )
    );
}

enum class NodeModuleness {
    Generic,
    Particle,
    FeedbackLoop,
};

static auto is_feedback_loop(NodeDefinition const& node_definition)
{
    return node_definition.name() == "FEEDBACK LOOP";
}

static auto node_moduleness(NodeDefinition const& node_definition)
{
    if (is_particle(node_definition.signature()))
        return NodeModuleness::Particle;
    if (is_feedback_loop(node_definition))
        return NodeModuleness::FeedbackLoop;
    return NodeModuleness::Generic;
}

void ModulesGraph::recreate_all_modules(Cool::NodeId const& root_node_id, DataToGenerateShaderCode const& data_to_generate_shader_code)
{
    _module_nodes.clear();
    _root_module_node = create_module(root_node_id, data_to_generate_shader_code);
}

auto ModulesGraph::create_module(Cool::NodeId const& root_node_id, DataToGenerateShaderCode const& data) -> std::shared_ptr<ModulesGraphNode>
{
    auto const* node = data.nodes_graph.try_get_node<Node>(root_node_id);
    if (!node)
        return nullptr; // TODO(FeedbackLoop) Return an error message?
    auto const* node_def = data.get_node_definition(node->id_names());
    if (!node_def)
        return nullptr; // TODO(FeedbackLoop) Return an error message?

    switch (node_moduleness(*node_def))
    {
    case NodeModuleness::Generic:
        return create_compositing_module(root_node_id, data);
    case NodeModuleness::Particle:
        return create_particles_module(root_node_id, *node_def, data);
    case NodeModuleness::FeedbackLoop:
        return create_feedback_loop_module(root_node_id, data);
    }
}

auto ModulesGraph::create_compositing_module(Cool::NodeId const& root_node_id, DataToGenerateShaderCode const& data) -> std::shared_ptr<ModulesGraphNode>
{
    auto dependencies = std::vector<std::shared_ptr<ModulesGraphNode>>{};

    auto const shader_code = generate_compositing_shader_code(
        root_node_id,
        [&](Cool::NodeId const& node_id, NodeDefinition const& node_definition) -> std::optional<std::string> {
            switch (node_moduleness(node_definition))
            {
            case NodeModuleness::Generic:
                return std::nullopt;
            case NodeModuleness::Particle:
            {
                dependencies.push_back(create_particles_module(node_id, node_definition, data));
                return dependencies.back()->texture_name_in_shader;
            }
            case NodeModuleness::FeedbackLoop:
            {
                dependencies.push_back(create_feedback_loop_module(node_id, data));
                return dependencies.back()->texture_name_in_shader;
            }
            }
        },
        [&]() {
            std::vector<std::string> tex_names;
            tex_names.reserve(_module_nodes.size());
            for (auto const& node : _module_nodes)
            {
                tex_names.push_back(node->texture_name_in_shader);
            }
            return tex_names;
        },
        data
    );

    auto module = std::make_shared<Module_Compositing>();
    module->set_shader_code(shader_code);

    _module_nodes.push_back(std::make_shared<ModulesGraphNode>(
        /* .module                 = */ module,
        /* .texture_name_in_shader = */ texture_name_for_module(root_node_id),
        /* .dependencies           = */ std::move(dependencies)
    ));
    return _module_nodes.back();
}

auto ModulesGraph::create_particles_module(Cool::NodeId const& root_node_id, NodeDefinition const& node_definition, DataToGenerateShaderCode const& data) -> std::shared_ptr<ModulesGraphNode>
{
    int const  dimension              = is_particle_3D(node_definition.signature()) ? 3 : 2;
    auto const texture_name_in_shader = texture_name_for_module(root_node_id);

    // TODO(FeedbackLoop) if a module with the same texture_name_in_shader already exists, return it
    // if (std::none_of(
    //         _particles_module_nodes.begin(),
    //         _particles_module_nodes.end(),
    //         [&](std::unique_ptr<ModulesGraphNode> const& node) {
    //             return node->texture_name_in_shader == texture_name_in_shader;
    //         }
    //     ))

    auto       id_of_node_storing_particles_count = Cool::NodeId{}; // Will be initialized by generate_simulation_shader_code()
    auto const simulation_shader_code             = generate_simulation_shader_code(
        root_node_id,
        id_of_node_storing_particles_count,
        dimension,
        data
    );

    auto module = std::make_shared<Module_Particles>(id_of_node_storing_particles_count);
    module->set_simulation_shader_code(simulation_shader_code, false, dimension);

    _module_nodes.push_back(std::make_shared<ModulesGraphNode>(
        /* .module                 = */ module,
        /* .texture_name_in_shader = */ texture_name_in_shader,
        /* .dependencies           = */ std::vector<std::shared_ptr<ModulesGraphNode>>{} // TODO(FeedbackLoop) Accumulate the dependencies while generating the shader code
    ));
    return _module_nodes.back();
}

auto ModulesGraph::create_feedback_loop_module(Cool::NodeId const& root_node_id, DataToGenerateShaderCode const& data) -> std::shared_ptr<ModulesGraphNode>
{
    auto module = std::make_shared<Module_FeedbackLoop>();

    auto const* node = data.nodes_graph.try_get_node<Node>(root_node_id);
    if (!node)
        return nullptr; // TODO(FeedbackLoop) Return an error message?

    assert(node->input_pins().size() == 1);
    auto const predecessor_node_id = data.nodes_graph.find_node_connected_to_input_pin(node->input_pins()[0].id());
    auto const dependency          = create_module(predecessor_node_id, data); // TODO(FeedbackLoop) Handle the case where the input pin is not connected to anything (generate a dummy module that returns a default texture? we could reuse the same dummy module when there is no node at all in the graph)

    _module_nodes.push_back(std::make_shared<ModulesGraphNode>(
        /* .module                 = */ module,
        /* .texture_name_in_shader = */ texture_name_for_module(root_node_id),
        /* .dependencies           = */ std::vector<std::shared_ptr<ModulesGraphNode>>{dependency}
    ));
    return _module_nodes.back();
}

void ModulesGraph::imgui_windows(Ui_Ref ui, Cool::AudioManager& audio_manager, Cool::NodesLibrary const& nodes_library) const
{
    for (auto const& node : _module_nodes)
        node->module->imgui_windows(ui);

    {
        auto cfg = Cool::NodesConfig{nodes_config(ui, audio_manager, nodes_library)};
        if (_nodes_editor.imgui_windows(cfg, nodes_library))
            regenerate_code_flag().set_dirty();
    }
    DebugOptions::show_generated_shader_code([&] {
        if (ImGui::BeginTabBar("Shaders Tabs", ImGuiTabBarFlags_None))
        {
            // TODO(FeedbackLoop)
            // if (ImGui::BeginTabItem("Compositing"))
            // {
            //     _compositing_module.imgui_show_generated_shader_code();
            //     ImGui::EndTabItem();
            // }
            // for (auto const& _particles_module : _particles_module_nodes)
            // {
            //     ImGui::PushID(&_particles_module);
            //     if (ImGui::BeginTabItem("Particle Simulation"))
            //     {
            //         static_cast<Module_Particles*>(_particles_module->module.get())->imgui_show_generated_shader_code();
            //         ImGui::EndTabItem();
            //     }
            //     ImGui::PopID();
            // }
            ImGui::EndTabBar();
        }
    });
}

static auto make_gizmo(Cool::SharedVariable<Cool::Point2D> const& var, CommandExecutor const& command_executor, Cool::Camera2D const& cam_2D) -> Cool::Gizmo_Point2D
{
    return Cool::Gizmo_Point2D{
        .get_position =
            [=]() { return Cool::ViewCoordinates{glm::vec2{cam_2D.view_matrix() * glm::vec3{var.value().value, 1.f}}}; },
        .set_position =
            [=](Cool::ViewCoordinates pos) {
                auto const world_pos = glm::vec2{cam_2D.transform_matrix() * glm::vec3{pos, 1.f}};
                command_executor.execute(Command_SetVariable<Cool::Point2D>{.var_ref = var.get_ref(), .value = Cool::Point2D{world_pos}});
            },
        .on_drag_stop =
            [=]() { command_executor.execute(Command_FinishedEditingVariable{}); },
        .id = var.id(),
    };
}

void ModulesGraph::submit_gizmos(Cool::GizmoManager& gizmos, CommandExecutor const& command_executor, Cool::Camera2D const& cam_2D)
{
    _nodes_editor.for_each_selected_node([&](Cool::Node const& node, Cool::NodeId const&) {
        for (auto const& var : node.downcast<Node>().value_inputs())
        {
            if (auto const* point_2D_var = std::get_if<Cool::SharedVariable<Cool::Point2D>>(&var))
            {
                gizmos.push(make_gizmo(*point_2D_var, command_executor, cam_2D));
            }
        }
    });
}

auto ModulesGraph::root_module() const -> Module const*
{
    return _root_module_node ? _root_module_node->module.get() : nullptr;
}

auto ModulesGraph::final_texture() const -> Cool::TextureRef
{
    return root_module() ? root_module()->texture() : Cool::TextureRef{};
}

auto ModulesGraph::nodes_config(Ui_Ref ui, Cool::AudioManager& audio_manager, Cool::NodesLibrary const& nodes_library) const -> NodesConfig
{
    return NodesConfig{
        Cool::GetNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetNodeCategoryConfig_Ref{nodes_library},
        ui,
        _main_node_id,
        _dirty_flags,
        _nodes_editor,
        audio_manager,
        ui.command_executor(),
    };
}

void ModulesGraph::on_time_changed()
{
    for (auto& node : _module_nodes)
    {
        node->module->on_time_changed();
        // TODO(FeedbackLoop)
        // if (node->module->depends_on().time)
        // {
        //     request_rerender_all(); // TODO(Modules) Only rerender the modules that depend on time
        // }
    }
}

void ModulesGraph::on_audio_changed()
{
    // TODO(FeedbackLoop) Reintroduce this
    // if (_compositing_module.depends_on().audio()
    //     || std::any_of(_particles_module_nodes.begin(), _particles_module_nodes.end(), [](auto const& module_node) { return module_node->module.depends_on().audio(); }))
    // {
    //     request_rerender_all(); // TODO(Modules) Only rerender the modules that depend on audio
    // }
}

void ModulesGraph::on_osc_channel_changed(Cool::OSCChannel const& osc_channel)
{
    // TODO(FeedbackLoop) Reintroduce this
    // if (_compositing_module.depends_on().osc_channel(osc_channel)
    //     || std::any_of(_particles_module_nodes.begin(), _particles_module_nodes.end(), [&](auto const& module_node) { return module_node->module.depends_on().osc_channel(osc_channel); }))
    // {
    //     request_rerender_all(); // TODO(Modules) Only rerender the modules that depend on this OSC channel
    // }
}

void ModulesGraph::on_midi_channel_changed(Cool::MidiChannel const& midi_channel)
{
    // TODO(FeedbackLoop) Reintroduce this
    // if (_compositing_module.depends_on().midi_channel(midi_channel)
    //     || std::any_of(_particles_module_nodes.begin(), _particles_module_nodes.end(), [&](auto const& module_node) { return module_node->module.depends_on().midi_channel(midi_channel); }))
    // {
    //     request_rerender_all(); // TODO(Modules) Only rerender the modules that depend on this Midi channel
    // }
}

void ModulesGraph::on_last_midi_button_pressed_changed()
{
    // TODO(FeedbackLoop) Reintroduce this
    // if (_compositing_module.depends_on().last_midi_button_pressed
    //     || std::any_of(_particles_module_nodes.begin(), _particles_module_nodes.end(), [&](auto const& module_node) { return module_node->module.depends_on().last_midi_button_pressed; }))
    // {
    //     request_rerender_all(); // TODO(Modules) Only rerender the modules that depend on this last_midi_button_pressed
    // }
}

void ModulesGraph::update_dependencies_from_nodes_graph()
{
    for (auto const& module_node : _module_nodes)
        module_node->module->update_dependencies_from_nodes_graph(_nodes_editor.graph());
}

void ModulesGraph::debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const
{
    ui.window({.name = "Nodes Registry"}, [&]() {
        imgui_show(_nodes_editor.graph().nodes());
    });
    ui.window({.name = "Links Registry"}, [&]() {
        imgui_show(_nodes_editor.graph().links());
    });
}

auto ModulesGraph::get_main_node_id() const -> Cool::NodeId const&
{
    return _main_node_id;
}
void ModulesGraph::set_main_node_id(Cool::NodeId const& id)
{
    _main_node_id = id;
    regenerate_code_flag().set_dirty(); // Important when calling this function from a Command
}

void ModulesGraph::add_node(Cool::NodeId const& id, Node const& node)
{
    _nodes_editor.graph().add_node(id, node);
    regenerate_code_flag().set_dirty(); // Important when calling this function from a Command
}

void ModulesGraph::add_link(Cool::LinkId const& id, Cool::Link const& link)
{
    _nodes_editor.graph().add_link(id, link);
    regenerate_code_flag().set_dirty(); // Important when calling this function from a Command
}

void ModulesGraph::remove_node(Cool::NodeId const& id)
{
    _nodes_editor.graph().nodes().with_mutable_ref(id, [&](Cool::Node& node) {
        node.downcast<Node>().clear_all_error_messages();
    });
    _nodes_editor.graph().remove_node(id);
    regenerate_code_flag().set_dirty(); // Important when calling this function from a Command
}

void ModulesGraph::remove_link(Cool::LinkId const& id)
{
    _nodes_editor.graph().remove_link(id);
    regenerate_code_flag().set_dirty(); // Important when calling this function from a Command
}

auto ModulesGraph::try_get_node(Cool::NodeId const& id) const -> Node const*
{
    return graph().try_get_node<Node>(id);
}

void ModulesGraph::set_node(Cool::NodeId const& id, Node const& value)
{
    graph().nodes().with_mutable_ref(id, [&](Cool::Node& node) {
        node.downcast<Node>() = value;
    });
    regenerate_code_flag().set_dirty(); // Important when calling this function from a Command
}

} // namespace Lab
