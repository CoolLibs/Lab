#include "ModulesGraph.h"
#include <Module_Particles/Module_Particles.h>
#include <Nodes/FunctionSignature.h>
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/Gpu/RenderTarget.h"
#include "Module_Compositing/generate_compositing_shader_code.h"
#include "Module_Particles/generate_simulation_shader_code.h"
#include "UI/imgui_show.h"

// TODO(Particles) Don't rerender everey frame (missing a set_clean(flag) somewhere)

namespace Lab {

ModulesGraph::ModulesGraph(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : _compositing_module{dirty_flag_factory, input_factory}
    // , _particles_module{dirty_flag_factory, input_factory}
    , _regenerate_code_flag{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(Cool::InputDefinition<Cool::Camera>{.name = "Camera"}, _regenerate_code_flag)} // TODO(Modules) Move this to the project, like the Camera2D // TODO(Modules) Shouldn't pass _regenerate_code_flag
{
}

void ModulesGraph::render(Cool::RenderTarget& render_target, Module::RenderParams in, UpdateContext_Ref update_ctx)
{
    if (render_target.needs_resizing())
        trigger_rerender_all(update_ctx.dirty_setter());

    if (in.is_dirty(_regenerate_code_flag))
    {
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::ToUser::info("Nodes", "Compiled");
        create_and_compile_all_modules(_nodes_editor.graph(), _main_node_id, update_ctx);
        in.set_clean(_regenerate_code_flag);
    }

    // _particles_render_target.set_size(render_target.desired_size());

    // TODO(Particles) Render in the order of dependency between the modules
    for (auto& module : _particles_modules)
        render_particle_module(*module, render_target, in, update_ctx); // TODO(Particles) Create a render target for each module and render on it here.
    render_compositing_module(render_target, in, update_ctx);
}

void ModulesGraph::render_particle_module(Module_Particles& module, Cool::RenderTarget& render_target, Module::RenderParams in, UpdateContext_Ref update_ctx)
{
    module._nodes_graph  = &_nodes_editor.graph();
    module._main_node_id = _main_node_id; // TODO(Particles) delete this

    // TODO(Particles) Rename module.is_dirty() as module.needs_to_render
    // TODO(Particles) Remove module.is_dirty() from module
    if (!module.is_dirty(in.is_dirty))
        return;

    if (DebugOptions::log_when_compiling_nodes())
        Cool::Log::ToUser::info("Nodes", "Rendered particles");
    // _compositing_module.compile(_nodes_editor.graph(), _main_node_id, update_ctx);
    // module.compile(_nodes_editor.graph(), _main_node_id, update_ctx);
    in.set_clean(module.dirty_flag());

    render_target.render([&]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        module.do_rendering(in, update_ctx);
    });
}

void ModulesGraph::render_one_module(Module& some_module, Cool::RenderTarget& render_target, Module::RenderParams params, UpdateContext_Ref update_ctx)
{
    render_target.render([&]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        some_module.do_rendering(params, update_ctx);
    });
    if (DebugOptions::log_when_rendering())
        Cool::Log::ToUser::info(some_module.name() + " Rendering", "Rendered");
}

void ModulesGraph::render_compositing_module(Cool::RenderTarget& render_target, Module::RenderParams in, UpdateContext_Ref update_ctx)
{
    _compositing_module._nodes_graph = &_nodes_editor.graph();

    // TODO(Particles) Rename module.is_dirty() as module.needs_to_render
    // TODO(Particles) Remove module.is_dirty() from module
    if (!_compositing_module.is_dirty(in.is_dirty))
        return;

    if (DebugOptions::log_when_compiling_nodes())
        Cool::Log::ToUser::info("Nodes", "Rendered compositing");
    // _compositing_module.compile(_nodes_editor.graph(), _main_node_id, update_ctx);
    // _particles_module.compile(_nodes_editor.graph(), _main_node_id, update_ctx);
    in.set_clean(_compositing_module.dirty_flag());

    if (_compositing_module.shader_is_valid())
    {
        _compositing_module.shader().bind();
        _compositing_module.shader().set_uniform_texture(
            "_particles_texture",
            _particles_render_target.get().texture_id(),
            Cool::TextureSamplerDescriptor{
                .repeat_mode        = Cool::TextureRepeatMode::None,
                .interpolation_mode = glpp::Interpolation::Linear,
            }
        );
        Cool::CameraShaderU::set_uniform(_compositing_module.shader(), in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{}));
    }

    render_one_module(_compositing_module, render_target, in, update_ctx);
}

void ModulesGraph::trigger_rerender_all(Cool::SetDirty_Ref set_dirty)
{
    set_dirty(_compositing_module.dirty_flag());
    for (auto& module : _particles_modules)
        set_dirty(module->dirty_flag());
}

void ModulesGraph::create_and_compile_all_modules(Cool::NodesGraph const& graph, Cool::NodeId const& root_node_id, UpdateContext_Ref ctx)
{
    _particles_modules.clear();
    _compositing_module.reset();

    if (!graph.try_get_node<Node>(root_node_id))
        return;
    auto const get_node_def = Cool::GetNodeDefinition_Ref<NodeDefinition>{ctx.nodes_library()};

    auto const shader_code = generate_compositing_shader_code(
        graph,
        root_node_id,
        get_node_def,
        ctx.input_provider(),
        [&](Cool::NodeId const& particles_root_node_id, NodeDefinition const& node_definition) {
            if (!is_particle(node_definition.signature()))
                return false;

            auto const simulation_shader_code = generate_simulation_shader_code(
                graph,
                particles_root_node_id,
                get_node_def,
                ctx.input_provider()
            );
            // TODO(Particles) Create the module and set its shader
            // _particle_modules.push_back(
            //     Module_Particles()
            // );
            // _particle_modules.back().set_simulation_shader_code(simulation_shader_code);
            return true;
        }
    );
    _compositing_module.set_shader_code(shader_code, ctx, false);

    // try
    // {
    //     _particle_system = Cool::ParticleSystem{
    //         _particles_count,
    //         Cool::ParticlesShadersCode{
    //             .simulation = _shader_code,
    //             .init       = *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp"),
    //             .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert"),
    //             .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
    //         }
    //     };
    // }
    // catch (std::exception const& e)
    // {
    //     Cool::Log::ToUser::error("Particles Compilation Failed", e.what());
    //     return;
    // }

    // TODO(Particles)
    // auto const maybe_err = _shader.compile(_shader_code, update_ctx);
    //
    // handle_error(maybe_err, for_testing_nodes);
    //
    // compute_dependencies();
}

void ModulesGraph::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    {
        auto cfg = Cool::NodesConfig{nodes_config(ui, update_ctx.nodes_library())};
        if (_nodes_editor.imgui_windows(cfg, update_ctx.nodes_library()))
        {
            // ui.set_dirty(_regenerate_code_flag);
            ui.set_dirty(_regenerate_code_flag);
        }
    }
    // _compositing_module.imgui_windows(ui, update_ctx);
    // _particles_module.imgui_windows(ui, update_ctx);
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
            return Cool::ViewCoordinates{glm::vec2{glm::inverse(cam_transform) * glm::vec3{var->value().value, 1.f}}};
            //
        },
        .set_position = [=](Cool::ViewCoordinates pos) {
            //
            auto const world_pos = glm::vec2{cam_transform * glm::vec3{pos, 1.f}};
            ctx.ui().command_executor().execute(
                Command_SetVariable<Cool::Point2D>{.id = id, .value = Cool::Point2D{world_pos}}
            );
            //
        },
        .on_drag_stop = [=]() {
            //
            ctx.ui().command_executor().execute(
                Command_FinishedEditingVariable{}
            );
            //
        },
        .id = id,
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

auto ModulesGraph::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return check_dirty(_regenerate_code_flag)
           || _compositing_module.is_dirty(check_dirty)
           || std::any_of(_particles_modules.begin(), _particles_modules.end(), [&](std::unique_ptr<Module_Particles> const& module) {
                  return module->is_dirty(check_dirty);
              });
};

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
        _compositing_module.shader_dirty_flag(), // TODO(Modules) Need to warn the particles_module too
        // _regenerate_code_flag,
        _regenerate_code_flag,
        _nodes_editor.graph(),
        ui.audio_manager(),
    };
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