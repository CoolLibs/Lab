#pragma once

#include <Cool/Gpu/Texture.h>
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/Editor.h>
#include "Common/FullscreenShader.h"
#include "Cool/Nodes/Graph.h"
#include "Cool/Nodes/NodesFolderWatcher.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Path/Path.h"
#include "Dependencies/Module.h"
#include "Module_Nodes/NodesConfig.h"
#include "NodesConfig.h"

namespace Lab {

class Module_Nodes : public Module {
public:
    Module_Nodes() = default;
    explicit Module_Nodes(Cool::DirtyFlagFactory_Ref, Cool::InputFactory_Ref);

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref, UpdateContext_Ref) const override;
    void submit_gizmos(Cool::GizmoManager&, UpdateContext_Ref) override;
    auto all_inputs() const -> Cool::AllInputRefsToConst override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void compile(UpdateContext_Ref, bool for_testing_nodes = false);

    void debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const;

    auto regenerate_code_flag() -> Cool::DirtyFlag& { return _regenerate_code_flag; }
    auto graph() -> Cool::Graph& { return _nodes_editor.graph(); }
    auto nodes_config(Ui_Ref, Cool::NodesLibrary&) const -> NodesConfig;

protected:
    void render(RenderParams, UpdateContext_Ref) override;

private:
    void handle_error(Cool::OptionalErrorMessage const&, bool for_testing_nodes) const;

private:
    mutable std::string         _shader_code{};
    mutable FullscreenShader    _shader{};
    mutable Cool::NodesEditor   _nodes_editor{};
    mutable Cool::NodeId        _main_node_id{};
    mutable Cool::NodeId        _node_we_might_want_to_restore_as_main_node_id{};
    Cool::DirtyFlag             _regenerate_code_flag;
    mutable Cool::MessageSender _shader_compilation_error{};
    Cool::Input<Cool::Camera>   _camera_input;

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
