#include "Module_Particles.h"

namespace Lab {

Module_Particles::Module_Particles(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Nodes", dirty_flag_factory}
{
}

void Module_Particles::update(UpdateContext_Ref)
{
}

void Module_Particles::update_particles(UpdateContext_Ref)
{
    _particle_system.update();
}

void Module_Particles::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
}

auto Module_Particles::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty);
};

void Module_Particles::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    // TODO(Particles) set the uniforms camera, time, dt, etc
    _particle_system.render();
}

} // namespace Lab
