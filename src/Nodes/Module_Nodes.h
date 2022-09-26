#pragma once

#include <Cool/Nodes/Editor.h>
#include "Common/FullscreenShader.h"
#include "Dependencies/Module.h"

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
    FullscreenShader            _shader;
    mutable Cool::Nodes::Editor _nodes_editor{"bla"};
};

} // namespace Lab
