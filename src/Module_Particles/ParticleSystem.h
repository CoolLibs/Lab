#pragma once
#include "Cool/Particles/ParticleSystem.h"

namespace Lab {

class ParticleSystem {
public:
    ParticleSystem();

    void imgui_debug_menu();
    void update() { _particle_system.update(); }
    void render() { _particle_system.render(); }

private:
    auto create_particle_system() const -> Cool::ParticleSystem;
    void recreate_particle_system(); // TODO(Modules) Remove me, this is for tests only

private:
    size_t               _particles_count{500};
    Cool::ParticleSystem _particle_system;
};

} // namespace Lab