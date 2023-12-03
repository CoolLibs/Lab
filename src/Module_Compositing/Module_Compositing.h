#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/Editor.h>
#include "Common/FullscreenShader.h"
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/RenderTarget.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Nodes/NodesFolderWatcher.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "Module/Module.h"
#include "NodesConfig.h"

namespace Lab {

class Module_Compositing : public Module {
public:
    Module_Compositing() = default;
    explicit Module_Compositing(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    void submit_gizmos(Cool::GizmoManager&, UpdateContext_Ref) override;
    auto all_inputs() const -> Cool::AllInputRefsToConst override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void compile(UpdateContext_Ref, bool for_testing_nodes = false);

    void debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const;

    auto               regenerate_code_flag() -> Cool::DirtyFlag& { return _regenerate_code_flag; }
    auto               graph() -> Cool::Graph& { return _nodes_editor.graph(); }
    auto               nodes_config(Ui_Ref, Cool::NodesLibrary&) const -> NodesConfig;
    [[nodiscard]] auto is_empty() const -> bool { return _nodes_editor.is_empty(); }

    [[nodiscard]] auto depends_on_time() const -> bool { return _depends_on_time; }
    [[nodiscard]] auto depends_on_particles() const -> bool { return _depends_on_particles; }
    [[nodiscard]] auto depends_on_audio() const -> bool { return _depends_on_audio_volume || _depends_on_audio_waveform || _depends_on_audio_spectrum; }

private:
    void render(RenderParams, UpdateContext_Ref) override;
    void handle_error(Cool::OptionalErrorMessage const&, bool for_testing_nodes) const;
    auto nodes_config(Ui_Ref ui) const -> NodesConfig;
    void render_impl(RenderParams, UpdateContext_Ref);
    void compute_dependencies(); // We don't want to rerender when audio / time changes if we don't depend on them. Also, audio features are costly to compute, so we only set these uniforms in the shader if we actually need them.

private:
    mutable std::string              _shader_code{};
    mutable FullscreenShader         _shader{};
    mutable Cool::NodesEditor        _nodes_editor{};
    mutable Cool::NodeId             _main_node_id{};
    mutable Cool::NodeId             _node_we_might_want_to_restore_as_main_node_id{};
    Cool::DirtyFlag                  _regenerate_code_flag;
    mutable Cool::MessageSender      _shader_compilation_error{};
    Cool::Input<Cool::Camera>        _camera_input;
    Cool::DoubleBufferedRenderTarget _feedback_double_buffer{};

    bool _depends_on_time{false};
    bool _depends_on_particles{false};
    bool _depends_on_audio_volume{false};
    bool _depends_on_audio_waveform{false};
    bool _depends_on_audio_spectrum{false};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Dirty Flag: Regenerate Code", _regenerate_code_flag),
            cereal::make_nvp("Shader", _shader),
            cereal::make_nvp("Node Editor", _nodes_editor),
            cereal::make_nvp("Main Node ID", _main_node_id),
            cereal::make_nvp("Camera Input", _camera_input)
        );
    }
};

} // namespace Lab
