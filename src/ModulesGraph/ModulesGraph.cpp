#include "ModulesGraph.h"
#include <Module_Particles/Module_Particles.h>
#include <Nodes/FunctionSignature.h>
#include <Nodes/module_texture_name.h>
#include <imgui.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/Gpu/RenderTarget.h"
#include "Module_Compositing/generate_compositing_shader_code.h"
#include "Module_Particles/generate_simulation_shader_code.h"
#include "UI/imgui_show.h"

namespace Lab {

ModulesGraph::ModulesGraph(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : _compositing_module{dirty_flag_factory, input_factory}
    , _regenerate_code_flag{dirty_flag_factory.make()}
    , _rerender_all_flag{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(Cool::InputDefinition<Cool::Camera>{.name = "Camera"}, _rerender_all_flag)} // TODO(Modules) Move this to the project, like the Camera2D
{
}

void ModulesGraph::render(Cool::RenderTarget& render_target, Module::RenderParams in, UpdateContext_Ref update_ctx, Cool::DirtyFlagFactory_Ref dirty_flag_factory)
{
    if (render_target.needs_resizing())
        trigger_rerender_all(update_ctx.dirty_setter());
    if (in.is_dirty(_rerender_all_flag))
    {
        trigger_rerender_all(update_ctx.dirty_setter());
        in.set_clean(_rerender_all_flag);
    }

    if (in.is_dirty(_regenerate_code_flag))
    {
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::ToUser::info("Modules Graph", "Compiled");
        create_and_compile_all_modules(_nodes_editor.graph(), _main_node_id, update_ctx, dirty_flag_factory);
        trigger_rerender_all(update_ctx.dirty_setter());
        in.set_clean(_regenerate_code_flag);
    }

    for (auto& module_node : _particles_module_nodes)
        module_node->render_target.set_size(render_target.desired_size());
    _compositing_module.set_render_target_size(render_target.desired_size()); // Must be done before rendering, otherwise we might read a target that is too small. (e.g. 1 pixel on app startup)

    // TODO(Particles) Remove those _nodes_graph
    for (auto& module_node : _particles_module_nodes)
    {
        module_node->module._nodes_graph            = &_nodes_editor.graph();
        module_node->module._camera_input           = &_camera_input;
        module_node->module._feedback_double_buffer = &_compositing_module.feedback_double_buffer();
        if (module_node->module.needs_to_rerender(in.is_dirty))
            update_ctx.set_dirty(_compositing_module.needs_to_rerender_flag());
    }
    _compositing_module._nodes_graph  = &_nodes_editor.graph();
    _compositing_module._camera_input = &_camera_input;
    // TODO(Modules) Render in the order of dependency between the modules
    for (auto& node : _particles_module_nodes)
        render_particle_module(node->module, node->render_target, in, update_ctx);
    render_compositing_module(render_target, in, update_ctx);
}

void ModulesGraph::render_one_module(Module& some_module, Cool::RenderTarget& render_target, Module::RenderParams params, UpdateContext_Ref update_ctx)
{
    // TODO(Modules) Rename module.is_dirty() as module.needs_to_render
    // TODO(Modules) Remove module.is_dirty() from module
    if (!some_module.needs_to_rerender(params.is_dirty))
        return;
    params.set_clean(some_module.needs_to_rerender_flag());

    render_target.render([&]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        some_module.do_rendering(params, update_ctx);
    });

    if (DebugOptions::log_when_rendering())
        Cool::Log::ToUser::info(some_module.name() + " Module", "Rendered");
}

void ModulesGraph::render_particle_module(Module_Particles& module, Cool::RenderTarget& render_target, Module::RenderParams in, UpdateContext_Ref update_ctx)
{
    render_one_module(module, render_target, in, update_ctx);
}

void ModulesGraph::render_compositing_module(Cool::RenderTarget& render_target, Module::RenderParams in, UpdateContext_Ref update_ctx)
{
    if (_compositing_module.shader_is_valid())
    {
        _compositing_module.shader().bind();

        for (auto const& module_node : _particles_module_nodes)
        {
            _compositing_module.shader().set_uniform_texture(
                module_node->texture_name_in_shader,
                module_node->render_target.get().texture_id(),
                Cool::TextureSamplerDescriptor{
                    .repeat_mode        = Cool::TextureRepeatMode::None,
                    .interpolation_mode = glpp::Interpolation::Linear,
                }
            );
        }
        // Cool::CameraShaderU::set_uniform(_compositing_module.shader(), in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{}));
    }

    render_one_module(_compositing_module, render_target, in, update_ctx);
}

void ModulesGraph::trigger_rerender_all(Cool::SetDirty_Ref set_dirty)
{
    set_dirty(_compositing_module.needs_to_rerender_flag());
    for (auto& node : _particles_module_nodes)
        set_dirty(node->module.needs_to_rerender_flag());
}

void ModulesGraph::on_time_reset()
{
    _compositing_module.on_time_reset();
    for (auto& node : _particles_module_nodes)
        node->module.on_time_reset();
}

void ModulesGraph::create_and_compile_all_modules(Cool::NodesGraph const& graph, Cool::NodeId const& root_node_id, UpdateContext_Ref ctx, Cool::DirtyFlagFactory_Ref dirty_flag_factory)
{
    _particles_module_nodes.clear();
    _compositing_module.reset_shader();

    if (!graph.try_get_node<Node>(root_node_id))
        return;
    auto const get_node_def = Cool::GetNodeDefinition_Ref<NodeDefinition>{ctx.nodes_library()};

    auto const shader_code = generate_compositing_shader_code(
        graph,
        root_node_id,
        get_node_def,
        ctx.input_provider(),
        [&](Cool::NodeId const& particles_root_node_id, NodeDefinition const& node_definition) -> std::optional<std::string> {
            if (!is_particle(node_definition.signature()))
                return std::nullopt;

            size_t dimension = 2;
            if (is_particle_3D(node_definition.signature()))
                dimension = 3;

            auto const simulation_shader_code = generate_simulation_shader_code(
                graph,
                particles_root_node_id,
                get_node_def,
                ctx.input_provider(),
                dimension
            );

            auto const texture_name_in_shader = module_texture_name(node_definition, particles_root_node_id);

            if (std::none_of(
                    _particles_module_nodes.begin(),
                    _particles_module_nodes.end(),
                    [&](std::unique_ptr<ModulesGraphNode> const& node) {
                        return node->texture_name_in_shader == texture_name_in_shader;
                    }
                ))
            {
                _particles_module_nodes.push_back(std::make_unique<ModulesGraphNode>(
                    /*  .module                 =*/Module_Particles(dirty_flag_factory, ctx.ui().input_factory()),
                    /*   .texture_name_in_shader = */ texture_name_in_shader
                ));

                _particles_module_nodes.back()->module.set_simulation_shader_code(simulation_shader_code, ctx, false, dimension);
            }

            return texture_name_in_shader;
        },
        [&]() {
            std::vector<std::string> tex_names;
            tex_names.reserve(_particles_module_nodes.size());
            for (auto const& node : _particles_module_nodes)
            {
                tex_names.push_back(node->texture_name_in_shader);
            }
            return tex_names;
        }
    );
    _compositing_module.set_shader_code(shader_code, ctx);
}

void ModulesGraph::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    for (auto const& _particles_module : _particles_module_nodes)
    {
        _particles_module->module.imgui_windows(ui, update_ctx);
    }
    _compositing_module.imgui_windows(ui, update_ctx);
    {
        auto cfg = Cool::NodesConfig{nodes_config(ui, update_ctx.nodes_library())};
        if (_nodes_editor.imgui_windows(cfg, update_ctx.nodes_library()))
        {
            ui.set_dirty(_regenerate_code_flag);
        }
    }
    DebugOptions::show_generated_shader_code([&] {
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("Compositing shader"))
            {
                _compositing_module.imgui_show_generated_shader_code(ui);
                ImGui::EndTabItem();
            }
            for (auto const& _particles_module : _particles_module_nodes)
            {
                ImGui::PushID(&_particles_module);
                if (ImGui::BeginTabItem("Particle shader"))
                {
                    _particles_module->module.imgui_show_generated_shader_code();
                    ImGui::EndTabItem();
                }
                ImGui::PopID();
            }
            ImGui::EndTabBar();
        }
    });
}

static auto make_gizmo(Cool::Input<Cool::Point2D> const& input, UpdateContext_Ref ctx) -> Cool::Gizmo_Point2D
{
    auto const cam_transform = ctx.input_provider()(Cool::Input_Camera2D{});
    auto const id            = input._default_variable_id.raw();
    return Cool::Gizmo_Point2D{
        .get_position = [=]() {
            auto const var = ctx.input_provider().variable_registries().get(id);
            if (!var)
                return Cool::ViewCoordinates{0.f};
            return Cool::ViewCoordinates{glm::vec2{glm::inverse(cam_transform) * glm::vec3{var->value().value, 1.f}}}; },
        .set_position = [=](Cool::ViewCoordinates pos) {
                auto const world_pos = glm::vec2{cam_transform * glm::vec3{pos, 1.f}};
                ctx.ui().command_executor().execute(
                        Command_SetVariable<Cool::Point2D>{.id = id, .value = Cool::Point2D{world_pos}}
                        ); },
        .on_drag_stop = [=]() { ctx.ui().command_executor().execute(
                                    Command_FinishedEditingVariable{}
                                ); },
        .id           = id,
    };
}

void ModulesGraph::submit_gizmos(Cool::GizmoManager& gizmos, UpdateContext_Ref ctx)
{
    _nodes_editor.for_each_selected_node([&](Cool::Node const& node) {
        for (auto const& input : node.downcast<Node>().value_inputs())
        {
            if (auto const* point_2D_input = std::get_if<Cool::Input<Cool::Point2D>>(&input))
            {
                gizmos.push(make_gizmo(*point_2D_input, ctx));
            }
        }
    });
}

auto ModulesGraph::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;

    inputs.push_back(Cool::AnyInputRefToConst{_camera_input});

    _nodes_editor.graph().for_each_node<Node>([&](Node const& node) {
        for (auto const& input : node.value_inputs())
        {
            inputs.push_back(std::visit([](auto&& input) { return Cool::AnyInputRefToConst{input}; }, input));
        }
    });

    return inputs;
}

auto ModulesGraph::nodes_config(Ui_Ref ui, Cool::NodesLibrary& nodes_library) const -> NodesConfig
{
    return NodesConfig{
        ui.input_factory(),
        Cool::GetNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetMutableNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetNodeCategoryConfig_Ref{nodes_library},
        ui,
        _main_node_id,
        _node_we_might_want_to_restore_as_main_node_id,
        _rerender_all_flag,
        _regenerate_code_flag,
        _nodes_editor.graph(),
        ui.audio_manager(),
    };
}

void ModulesGraph::on_time_changed(UpdateContext_Ref update_ctx)
{
    for (auto& node : _particles_module_nodes)
    {
        node->module._nodes_graph  = &_nodes_editor.graph();
        node->module._camera_input = &_camera_input;
        node->module.update_particles(update_ctx);
    }
    if (_compositing_module.depends_on_time()
        || !_particles_module_nodes.empty())
    {
        trigger_rerender_all(update_ctx.dirty_setter()); // TODO(Modules) Only rerender the modules that depend on time
    }
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

} // namespace Lab
