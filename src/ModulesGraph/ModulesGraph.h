#pragma once
#include "Cool/View/GizmoManager.h"
#include "Module_Compositing/Module_Compositing.h"
#include "Module_Particles/Module_Particles.h"
#include "Nodes/NodesConfig.h"

namespace Lab {

/// The main class containing all the nodes of the project.
/// It is responsible for spawning the various modules as required by the nodes, and knowing the dependencies between them.
class ModulesGraph {
public:
    ModulesGraph() = default;
    ModulesGraph(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void render(Module::RenderParams params, UpdateContext_Ref update_ctx);
    void trigger_rerender_all(Cool::SetDirty_Ref);

    [[nodiscard]] auto is_empty() const -> bool { return _nodes_editor.is_empty(); }
    auto               graph() -> Cool::NodesGraph& { return _nodes_editor.graph(); }               // TODO(Modules) Is this needed?
    auto               regenerate_code_flag() -> Cool::DirtyFlag& { return _regenerate_code_flag; } // TODO(Modules) Is this needed?
    auto               nodes_config(Ui_Ref, Cool::NodesLibrary&) const -> NodesConfig;
    void               debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const;
    void               update_particles(UpdateContext_Ref update_ctx) { _particles_module.update_particles(update_ctx); }

    void imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const;
    void submit_gizmos(Cool::GizmoManager&, UpdateContext_Ref);

    auto all_inputs() const -> Cool::AllInputRefsToConst;
    auto is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool;

    auto compositing_module() const -> Module_Compositing const& { return _compositing_module; }
    auto compositing_module() -> Module_Compositing& { return _compositing_module; }
    auto particles_module() const -> Module_Particles const& { return _particles_module; }
    auto particles_module() -> Module_Particles& { return _particles_module; }

private:
    void recreate_modules_graph();

private:
    mutable Cool::NodesEditor _nodes_editor{};
    mutable Cool::NodeId      _main_node_id{}; // TODO(Modules) Rename as _root_node_id? Or _output_node_id?
    mutable Cool::NodeId      _node_we_might_want_to_restore_as_main_node_id{};
    Cool::DirtyFlag           _regenerate_code_flag{}; // TODO(Modules) Rename as graph_has_changed_flag
    Cool::Input<Cool::Camera> _camera_input{};         // TODO(Modules) Does it belong here?

    Module_Compositing _compositing_module{};
    Module_Particles   _particles_module{}; // TODO(Modules) Only create one (or several) when the nodes graph needs them.

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Compositing Module", _compositing_module),
            cereal::make_nvp("Particles Module", _particles_module),
            cereal::make_nvp("Dirty Flag: Regenerate Code", _regenerate_code_flag),
            cereal::make_nvp("Node Editor", _nodes_editor),
            cereal::make_nvp("Main Node ID", _main_node_id),
            cereal::make_nvp("Camera Input", _camera_input)
        );
    }
};

} // namespace Lab