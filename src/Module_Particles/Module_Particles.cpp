#include "Module_Particles.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Particles/ParticleSystem.h"

namespace Lab {

Module_Particles::Module_Particles()
    : _particle_system{create_particle_system()}
{}

Module_Particles::Module_Particles(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Nodes", dirty_flag_factory}
    , _particle_system{create_particle_system()}
{
}

auto Module_Particles::create_particle_system() const -> std::optional<Cool::ParticleSystem>
{
    try
    {
        return Cool::ParticleSystem{
            _particles_count,
            Cool::ParticlesShadersCode{
                .simulation = *Cool::File::to_string(Cool::Path::root() / "res/Particles/simulation.comp"),
                .init = *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp"),
                .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert"),
                .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
            }
        };
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("Particles Compilation Failed", e.what());
        return {};
    }
}

void Module_Particles::imgui_debug_menu()
{
    if (ImGui::DragScalar("Particles Count", ImGuiDataType_U64, &_particles_count))
        recreate_particle_system();
    if (ImGui::Button("Recreate Particle System"))
        recreate_particle_system();
}

void Module_Particles::recreate_particle_system()
{
    _particle_system = create_particle_system();
}

void Module_Particles::update(UpdateContext_Ref)
{
}

void Module_Particles::update_particles(UpdateContext_Ref)
{
    if (!_particle_system)
        return;

    _particle_system->update();
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
    if (!_particle_system)
        return;

    _particle_system->simulation_shader().bind();
    _particle_system->simulation_shader().set_uniform("_time", in.provider(Cool::Input_Time{}));
    _particle_system->simulation_shader().set_uniform("_delta_time", in.provider(Cool::Input_DeltaTime{}));
    _particle_system->simulation_shader().set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    _particle_system->simulation_shader().set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    _particle_system->simulation_shader().set_uniform("_height", in.provider(Cool::Input_Height{}));
    _particle_system->simulation_shader().set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    _particle_system->render_shader().bind();
    _particle_system->render_shader().set_uniform("_time", in.provider(Cool::Input_Time{}));
    _particle_system->render_shader().set_uniform("_delta_time", in.provider(Cool::Input_DeltaTime{}));
    _particle_system->render_shader().set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    _particle_system->render_shader().set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    _particle_system->render_shader().set_uniform("_height", in.provider(Cool::Input_Height{}));
    _particle_system->render_shader().set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    _particle_system->render();
}

} // namespace Lab
