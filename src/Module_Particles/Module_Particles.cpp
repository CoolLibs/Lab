#include "Module_Particles.h"
#include <string>

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
    _particle_system.simulation_shader().bind();
    _particle_system.simulation_shader().set_uniform("_time", in.provider(Cool::Input_Time{}));
    _particle_system.simulation_shader().set_uniform("_delta_time", in.provider(Cool::Input_DeltaTime{}));
    _particle_system.simulation_shader().set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    _particle_system.simulation_shader().set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    _particle_system.simulation_shader().set_uniform("_height", in.provider(Cool::Input_Height{}));
    _particle_system.simulation_shader().set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    _particle_system.render_shader().bind();
    _particle_system.render_shader().set_uniform("_time", in.provider(Cool::Input_Time{}));
    _particle_system.render_shader().set_uniform("_delta_time", in.provider(Cool::Input_DeltaTime{}));
    _particle_system.render_shader().set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    _particle_system.render_shader().set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    _particle_system.render_shader().set_uniform("_height", in.provider(Cool::Input_Height{}));
    _particle_system.render_shader().set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    _particle_system.render();
}

} // namespace Lab
