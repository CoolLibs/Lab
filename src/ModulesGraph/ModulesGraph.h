#pragma once
#include "Cool/View/GizmoManager.h"
#include "Module_Compositing/Module_Compositing.h"
#include "Module_Particles/Module_Particles.h"
#include "Nodes/NodesConfig.h"

namespace Lab {

struct ModulesGraphNode {
    ModulesGraphNode() = default;
    // ~ModulesGraphNode() = default;

    // ModulesGraphNode(const ModulesGraphNode&)                      = delete; // We disable copying
    // ModulesGraphNode& operator=(const ModulesGraphNode&)           = delete; // We disable copying
    // ModulesGraphNode(ModulesGraphNode&& other) noexcept            = default;
    // ModulesGraphNode& operator=(ModulesGraphNode&& other) noexcept = default;

    ModulesGraphNode(Module_Particles module, std::string texture_name_in_shader)
        : module{std::move(module)}
        , texture_name_in_shader{std::move(texture_name_in_shader)}
    {
    }

    Module_Particles   module{};
    std::string        texture_name_in_shader{};
    Cool::RenderTarget render_target{};

private:
    // TODO(Particles) Think about how we serialize this
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Module", module)
        );
    }
};

/// The main class containing all the nodes of the project.
/// It is responsible for spawning the various modules as required by the nodes, and knowing the dependencies between them.
class ModulesGraph {
public:
    ModulesGraph() = default;
    ModulesGraph(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void update(UpdateContext_Ref);
    void render(Cool::RenderTarget&, Module::RenderParams, UpdateContext_Ref, Cool::DirtyFlagFactory_Ref dirty_flag_factory);
    void trigger_rerender_all(Cool::SetDirty_Ref);

    [[nodiscard]] auto is_empty() const -> bool { return _nodes_editor.is_empty(); }
    auto               graph() -> Cool::NodesGraph& { return _nodes_editor.graph(); }               // TODO(Modules) Is this needed?
    auto               regenerate_code_flag() -> Cool::DirtyFlag& { return _regenerate_code_flag; } // TODO(Modules) Is this needed?
    auto               nodes_config(Ui_Ref, Cool::NodesLibrary&) const -> NodesConfig;
    void               debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const;
    /// Function called once on every frame where the time has changed.
    void on_time_changed(UpdateContext_Ref);
    /// Function called once on every frame where the audio has changed.
    void on_audio_changed(UpdateContext_Ref);

    void imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const;
    void submit_gizmos(Cool::GizmoManager&, UpdateContext_Ref);

    auto all_inputs() const -> Cool::AllInputRefsToConst;

    auto compositing_module() const -> Module_Compositing const& { return _compositing_module; } // TODO(Modules) Remove
    auto compositing_module() -> Module_Compositing& { return _compositing_module; }             // TODO(Modules) Remove

    void on_time_reset();

private:
    void create_and_compile_all_modules(Cool::NodesGraph const&, Cool::NodeId const& root_node_id, UpdateContext_Ref, Cool::DirtyFlagFactory_Ref dirty_flag_factory);
    void render_one_module(Module&, Cool::RenderTarget&, Module::RenderParams params);
    void render_compositing_module(Cool::RenderTarget& render_target, Module::RenderParams params);
    void render_particle_module(Module_Particles&, Cool::RenderTarget& render_target, Module::RenderParams params);

private:
    mutable Cool::NodesEditor _nodes_editor{};
    mutable Cool::NodeId      _main_node_id{}; // TODO(Modules) Rename as _root_node_id? Or _output_node_id?
    mutable Cool::NodeId      _node_we_might_want_to_restore_as_main_node_id{};
    Cool::DirtyFlag           _regenerate_code_flag{}; // TODO(Modules) Rename as graph_has_changed_flag
    Cool::DirtyFlag           _rerender_all_flag{};
    Cool::Input<Cool::Camera> _camera_input{}; // TODO(Modules) Does it belong here?

    Module_Compositing                             _compositing_module{};
    std::vector<std::unique_ptr<ModulesGraphNode>> _particles_module_nodes{}; // TODO(Particles) No need for the unique_ptr (in theory)

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Compositing Module", _compositing_module),
            cereal::make_nvp("Particles Module", _particles_module_nodes),
            cereal::make_nvp("Dirty Flag: Regenerate Code", _regenerate_code_flag),
            cereal::make_nvp("Dirty Flag: Rerender all", _rerender_all_flag),
            cereal::make_nvp("Node Editor", _nodes_editor),
            cereal::make_nvp("Main Node ID", _main_node_id),
            cereal::make_nvp("Camera Input", _camera_input)
        );
    }
};

} // namespace Lab
