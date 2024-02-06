#pragma once
#include <Meshing/Meshing_Handler.h>
#include "Cool/Nodes/Editor.h"
#include "Cool/OSC/OSCChannel.h"
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

    void update();
    void render(Cool::RenderTarget&, SystemValues const&, Cool::NodesLibrary const&);

    void request_rerender_all();

    [[nodiscard]] auto is_empty() const -> bool { return _nodes_editor.is_empty(); }
    [[nodiscard]] auto graph() -> Cool::NodesGraph& { return _nodes_editor.graph(); }
    [[nodiscard]] auto regenerate_code_flag() -> Cool::DirtyFlag const& { return _regenerate_code_flag; }
    [[nodiscard]] auto rerender_all_flag() -> Cool::DirtyFlag const& { return _rerender_all_flag; }
    [[nodiscard]] auto nodes_config(Ui_Ref, Cool::AudioManager&, Cool::NodesLibrary const&) const -> NodesConfig;
    void               debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const;
    /// Function called once on every frame where the time has changed.
    void on_time_changed();
    void on_time_reset();
    /// Function called once on every frame where the audio has changed.
    void on_audio_changed();
    /// Function called once on every frame for each OSC channel that has changed.
    void on_osc_channel_changed(Cool::OSCChannel const&);
    /// Function called once on every frame for each Midi channel that has changed.
    void on_midi_channel_changed(Cool::MidiChannel const&);

    void update_dependencies_from_nodes_graph();

    void imgui_windows(Ui_Ref, Cool::AudioManager&, Cool::NodesLibrary const&) const;
    void submit_gizmos(Cool::GizmoManager&, CommandExecutor const&, Cool::Camera2D const&);

private:
    void create_and_compile_all_modules(Cool::NodesGraph const&, Cool::NodeId const& root_node_id, Cool::NodesLibrary const&);
    void render_one_module(Module&, Cool::RenderTarget&, SystemValues const&);
    void render_compositing_module(Cool::RenderTarget&, SystemValues const&);
    void render_particle_module(Module_Particles&, Cool::RenderTarget&, SystemValues const&);

private:
    mutable Cool::NodesEditor _nodes_editor{};
    mutable Cool::NodeId      _main_node_id{}; // TODO(Modules) Rename as _root_node_id? Or _output_node_id?
    mutable Cool::NodeId      _node_we_might_want_to_restore_as_main_node_id{};
    Cool::DirtyFlag           _regenerate_code_flag{}; // TODO(Modules) Rename as graph_has_changed_flag
    Cool::DirtyFlag           _rerender_all_flag{};

    mutable Module_Compositing                     _compositing_module{};
    std::vector<std::unique_ptr<ModulesGraphNode>> _particles_module_nodes{}; // TODO(Particles) No need for the unique_ptr (in theory)
    Meshing_Handler                                _meshing_handler{}; // TODO(Meshing) Move elsewhere

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
            cereal::make_nvp("Main Node ID", _main_node_id)
        );
    }
};

} // namespace Lab
