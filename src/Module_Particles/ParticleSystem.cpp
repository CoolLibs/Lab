#include "ParticleSystem.h"
#include "Cool/File/File.h"
#include "Cool/Particles/ParticleSystem.h"

namespace Lab {

auto ParticleSystem::create_particle_system() const -> Cool::ParticleSystem
{
    return Cool::ParticleSystem{
        _particles_count,
        Cool::ParticlesShadersCode{
            .simulation = *Cool::File::to_string(Cool::Path::root() / "res/Particles/simulation.comp"),
            .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert"),
            .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
        }
    };
}

ParticleSystem::ParticleSystem()
    : _particle_system{create_particle_system()}
{
}

void ParticleSystem::imgui_debug_menu()
{
    if (ImGui::DragScalar("Particles Count", ImGuiDataType_U64, &_particles_count))
        recreate_particle_system();
    if (ImGui::Button("Recreate Particle System"))
        recreate_particle_system();
}

void ParticleSystem::recreate_particle_system()
{
    _particle_system = create_particle_system();
}

} // namespace Lab