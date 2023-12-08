#include "Module_Particles.h"
#include <Nodes/shader_set_uniforms.h>
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Cool/String/String.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Nodes/Node.h"
#include "generate_simulation_shader_code.h"

namespace Lab {
Module_Particles::Module_Particles()
{
}

Module_Particles::Module_Particles(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Nodes", dirty_flag_factory}
{
}

void Module_Particles::reset()
{
    if(_particle_system.has_value())
        _particle_system->reset();
}

void Module_Particles::set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    if (!shader_code)
    {
        // ? TODO(Particles): handle error (parent class with Compositing ?)
        return;
    }

    _shader_code = *shader_code;

    // ? TODO(Particles): handle error (parent class with Compositing ?)
    // auto const maybe_err = _shader.compile(_shader_code, update_ctx);

    // TODO(Particles) Don't recreate the particle system every time, just change  the shader but keep the current position and velocity of the particles
    // _particle_system.set_simulation_shader(*shader_code); // TODO(Particles) Make this function return an optional error

    try
    {
        if (_particle_system.has_value())
        {
            _particle_system->set_simulation_shader(_shader_code);
        }
        else
        {
            _particle_system = Cool::ParticleSystem{
                _particles_count,
                Cool::ParticlesShadersCode{
                    .simulation = *shader_code,
                    .init       = *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp"),
                    .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert"),
                    .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
                }
            };

            // ? TODO(Particles): compute_dependencies (parent class with Compositing ?)
            compute_dependencies();
        }
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("Particles Compilation Failed", e.what());
        return;
    }
}

void Module_Particles::imgui_debug_menu(Cool::SetDirty_Ref set_dirty)
{
    // if (ImGui::DragScalar("Particles Count", ImGuiDataType_U64, &_particles_count))
    //     set_dirty(_regenerate_code_flag);
}

// void Module_Particles::recreate_particle_system()
// {
//     _particle_system = create_particle_system();
// }

void Module_Particles::update(UpdateContext_Ref)
{
}

void Module_Particles::update_particles(UpdateContext_Ref)
{
    if (!_particle_system)
        return;

    if (DebugOptions::log_when_updating_particles())
        Cool::Log::ToUser::info(name() + " Updating particles", "Particles updated");

    _particle_system->update();
}

void Module_Particles::compute_dependencies()
{
    auto const code = Cool::String::remove_comments(_shader_code);
    _dependencies.compute_dependencies(code);
}

void Module_Particles::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    DebugOptions::show_generated_shader_code([&] {
        ImGui::SeparatorText("Particle shader");
        if (Cool::ImGuiExtras::input_text_multiline("##Particles shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
        {
        }
    });
}

auto Module_Particles::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty);
};

void Module_Particles::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    if (!_particle_system)
        return;

    shader_set_uniforms(_particle_system->simulation_shader(), in, _dependencies, *_feedback_double_buffer);
    shader_send_uniforms(_particle_system->simulation_shader(), in, _nodes_graph);

    shader_set_uniforms(_particle_system->render_shader(), in, _dependencies, *_feedback_double_buffer);
    shader_send_uniforms(_particle_system->render_shader(), in, _nodes_graph);

    _particle_system->render();
}

} // namespace Lab
