#include "Module_Nodes.h"

namespace Lab {

Module_Nodes::Module_Nodes(Cool::DirtyFlagFactory_Ref dirty_flag_factory)
    : Module{"Nodes", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
{
}

void Module_Nodes::update(UpdateContext_Ref)
{
}

void Module_Nodes::imgui_windows(Ui_Ref) const
{
    // TODO(JF) Use `ui`
}

auto Module_Nodes::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;
    return inputs;
}

auto Module_Nodes::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty) ||
           check_dirty(_shader.dirty_flag());
};

void Module_Nodes::render(RenderParams in, UpdateContext_Ref)
{
    if (!_shader.pipeline().shader())
        return;

    const Cool::FullscreenPipeline& pipeline = _shader.pipeline();
    const Cool::OpenGL::Shader&     shader   = *pipeline.shader();

    shader.bind();
    shader.set_uniform("_time", in.provider(Cool::Input_Time{}));

    pipeline.draw();
}

} // namespace Lab
