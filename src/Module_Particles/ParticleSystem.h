#pragma once
#include "Cool/Particles/ParticleSystem.h"

namespace Lab {

class ParticleSystem {
public:
    ParticleSystem();

    void imgui_debug_menu();
    void update() { _particle_system.update(); }
    void render() { _particle_system.render(); }
    auto simulation_shader() -> Cool::OpenGL::ComputeShader& { return _particle_system.simulation_shader(); }
    auto simulation_shader() const -> Cool::OpenGL::ComputeShader const& { return _particle_system.simulation_shader(); }
    auto render_shader() -> Cool::OpenGL::Shader& { return _particle_system.render_shader(); }
    auto render_shader() const -> Cool::OpenGL::Shader const& { return _particle_system.render_shader(); }

private:
    auto create_particle_system() const -> Cool::ParticleSystem;
    void recreate_particle_system(); // TODO(Modules) Remove me, this is for tests only

private:
    size_t               _particles_count{500};
    Cool::ParticleSystem _particle_system;
};

} // namespace Lab