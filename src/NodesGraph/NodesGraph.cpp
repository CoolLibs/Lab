#include "NodesGraph.h"
#include <Module_Particles/Module_Particles.h>
#include "UI/imgui_show.h"

namespace Lab {

NodesGraph::NodesGraph(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : _compositing_module{dirty_flag_factory, input_factory}
    , _particles_module{dirty_flag_factory, input_factory}
    , _regenerate_code_flag{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(Cool::InputDefinition<Cool::Camera>{.name = "Camera"}, _regenerate_code_flag)} // TODO(Particles) Move this to the project, like the Camera2D // TODO(Particles) Shouldn't pass _regenerate_code_flag
{
}

void NodesGraph::render(Module::RenderParams in, UpdateContext_Ref update_ctx)
{
    if (in.is_dirty(_regenerate_code_flag))
    {
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::ToUser::info("Nodes", "Compiled");
        _compositing_module.compile(_nodes_editor.graph(), _main_node_id, update_ctx);
        in.set_clean(_regenerate_code_flag);
    }
    _particles_module.do_rendering(in, update_ctx);
    _compositing_module.do_rendering(in, update_ctx);
}

void NodesGraph::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    {
        auto cfg = Cool::NodesConfig{nodes_config(ui, update_ctx.nodes_library())};
        if (_nodes_editor.imgui_windows(cfg, update_ctx.nodes_library()))
            ui.set_dirty(_regenerate_code_flag);
    }
    _compositing_module.imgui_windows(ui, update_ctx);
    _particles_module.imgui_windows(ui, update_ctx);
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

void NodesGraph::submit_gizmos(Cool::GizmoManager& gizmos, UpdateContext_Ref ctx)
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

auto NodesGraph::all_inputs() const -> Cool::AllInputRefsToConst
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

auto NodesGraph::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return check_dirty(_regenerate_code_flag)
           || _compositing_module.is_dirty(check_dirty)
           || _particles_module.is_dirty(check_dirty);
};

auto NodesGraph::nodes_config(Ui_Ref ui, Cool::NodesLibrary& nodes_library) const -> NodesConfig
{
    return NodesConfig{
        ui.input_factory(),
        Cool::GetNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetMutableNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetNodeCategoryConfig_Ref{nodes_library},
        ui,
        _main_node_id,
        _node_we_might_want_to_restore_as_main_node_id,
        _compositing_module.shader_dirty_flag(), // TODO(Particles) Need to warn the particles_module too
        _regenerate_code_flag,
        _nodes_editor.graph(),
        ui.audio_manager(),
    };
}

void NodesGraph::debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const
{
    ui.window({.name = "Nodes Registry"}, [&]() {
        imgui_show(_nodes_editor.graph().nodes());
    });
    ui.window({.name = "Links Registry"}, [&]() {
        imgui_show(_nodes_editor.graph().links());
    });
}

} // namespace Lab