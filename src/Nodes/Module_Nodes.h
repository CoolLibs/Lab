#pragma once

#include <Cool/Nodes/Editor.h>
#include "Common/FullscreenShader.h"
#include "Dependencies/Module.h"
#include "NodesConfig.h"

namespace Lab {

class Module_Nodes : public Module {
public:
    Module_Nodes() = default;
    explicit Module_Nodes(Cool::DirtyFlagFactory_Ref);

    void update(UpdateContext_Ref) override;
    void imgui_windows(Ui_Ref) const override;
    auto all_inputs() const -> Cool::AllInputRefsToConst override;
    auto is_dirty(Cool::IsDirty_Ref) const -> bool override;

protected:
    void render(RenderParams, UpdateContext_Ref) override;

private:
    FullscreenShader                       _shader;
    mutable Cool::NodesEditor<NodesConfig> _nodes_editor{"Nodes Editor"};
    Cool::NodesLibrary<NodeDefinition>     _nodes_library;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
            cereal::make_nvp("Node Editor", _nodes_editor)
        );
    }
};

} // namespace Lab
