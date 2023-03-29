#pragma once

#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/Editor.h>
#include "Common/FullscreenShader.h"
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
    void imgui_windows(Ui_Ref) const override;
    auto all_inputs() const -> Cool::AllInputRefsToConst override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void compile(UpdateContext_Ref update_ctx, bool for_testing_nodes = false);

    void debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const;

protected:
    void render(RenderParams, UpdateContext_Ref) override;

private:
    void handle_error(Cool::OptionalErrorMessage const&, bool for_testing_nodes);
    auto nodes_config(Ui_Ref ui) const -> NodesConfig;

private:
    mutable std::string                                              _shader_code{};
    FullscreenShader                                                 _shader{};
    mutable Cool::NodesEditor                                        _nodes_editor{};
    mutable Cool::NodesLibrary                                       _nodes_library{};
    mutable /*TODO(JF) remove the mutable*/ Cool::NodesFolderWatcher _nodes_folder_watcher{Cool::Path::root() / "Nodes", ".clbnode"};
    mutable Cool::NodeId                                             _main_node_id{};
    Cool::DirtyFlag                                                  _regenerate_code_flag;
    Cool::MessageSender                                              _shader_compilation_error{};
    Cool::Input<Cool::Camera>                                        _camera_input;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Dirty Flag: Regenerate Code", _regenerate_code_flag),
            cereal::make_nvp("Shader", _shader)
#if COOL_SERIALIZATION
                ,
            cereal::make_nvp("Node Editor", _nodes_editor),
            cereal::make_nvp("Main Node ID", _main_node_id),
            cereal::make_nvp("Camera Input", _camera_input)
#endif
        );
    }
};

} // namespace Lab
