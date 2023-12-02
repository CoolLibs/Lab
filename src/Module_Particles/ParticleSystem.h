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
    void recreate_particle_system(); // TODO(Particles) Remove me, this is for tests only

private:
    Cool::ParticleSystem _particle_system;
};

} // namespace Lab