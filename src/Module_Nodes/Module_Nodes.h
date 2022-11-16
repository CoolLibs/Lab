#pragma once

#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/Editor.h>
#include "Common/FullscreenShader.h"
#include "Dependencies/Module.h"
#include "NodesConfig.h"
#include "NodesLibrary.h"

namespace Lab {

class Module_Nodes : public Module {
public:
    Module_Nodes() = default;
    explicit Module_Nodes(Cool::DirtyFlagFactory_Ref);

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref) const override;
    auto all_inputs() const -> Cool::AllInputRefsToConst override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

    void compile(UpdateContext_Ref update_ctx, bool for_testing_nodes = false);

protected:
    void render(RenderParams, UpdateContext_Ref) override;

private:
    void handle_error(Cool::OptionalErrorMessage const&, bool for_testing_nodes);

private:
    mutable std::string                    _shader_code{};
    FullscreenShader                       _shader{};
    mutable Cool::NodesEditor<NodesConfig> _nodes_editor{"Nodes Editor"};
    NodesLibrary                           _nodes_library{};
    mutable Cool::NodeId                   _main_node_id{};
    Cool::DirtyFlag                        _regenerate_code_flag;
    Cool::MessageSender                    _shader_compilation_error{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Node Editor", _nodes_editor),
            cereal::make_nvp("Main Node ID", _main_node_id),
            cereal::make_nvp("Dirty Flag: Regenerate Code", _regenerate_code_flag),
            cereal::make_nvp("Shader", _shader)
        );
    }
};

} // namespace Lab
