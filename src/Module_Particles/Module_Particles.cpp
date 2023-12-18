#include "Module_Particles.h"
#include <Nodes/shader_set_uniforms.h>
#include <imgui.h>
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/Exception/Exception.h"
#include "Cool/File/File.h"
#include "Cool/Log/OptionalErrorMessage.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Cool/String/String.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Module/ShaderBased/handle_error.h"
#include "Nodes/Node.h"
#include "generate_simulation_shader_code.h"

namespace Lab {

Module_Particles::Module_Particles()
{
}

Module_Particles::Module_Particles(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Particles", dirty_flag_factory}
{
}

void Module_Particles::on_time_reset()
{
    if (_particle_system.has_value())
        _particle_system->reset();
}

void Module_Particles::set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx, bool for_testing_nodes, size_t dimension)
{
    if (!shader_code)
    {
        handle_error(Cool::OptionalErrorMessage{shader_code.error()}, name(), _shader_compilation_error);
        return;
    }

    _shader_code = *shader_code;
    _shader_compilation_error.clear();

    // TODO(Particles) Don't recreate the particle system every time, just change  the shader but keep the current position and velocity of the particles
    try
    {
        if (_particle_system.has_value())
        {
            _particle_system->set_simulation_shader(_shader_code);
        }
        else
        {
            assert(dimension == 2 || dimension == 3);
            std::string init   = (dimension == 3 ? "#define IS_3D\n" : "") + *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp");
            std::string vertex = std::string("#version 430\n") + (dimension == 3 ? "#define IS_3D\n" : "") + *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert");

            _particle_system = Cool::ParticleSystem{
                _particles_count,
                Cool::ParticlesShadersCode{
                    .simulation = *shader_code,
                    .init       = init,
                    .vertex     = vertex,
                    .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
                }
            };
            // switch (dimension)
            // {
            // case 2:
            //     _particle_system = Cool::ParticleSystem{
            //         _particles_count,
            //         Cool::ParticlesShadersCode{
            //             .simulation = *shader_code,
            //             .init       = *Cool::File::to_string(Cool::Path::root() / "res/Particles/init2.comp"),
            //             .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex2.vert"),
            //             .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
            //         }
            //     };
            //     break;
            // case 3:
            //     _particle_system = Cool::ParticleSystem{
            //         _particles_count,
            //         Cool::ParticlesShadersCode{
            //             .simulation = *shader_code,
            //             .init       = *Cool::File::to_string(Cool::Path::root() / "res/Particles/init3.comp"),
            //             .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex3.vert"),
            //             .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
            //         }
            //     };
            //     break;
            // default: break;
            // }

            // TODO(Particles): compute_dependencies (parent class with Compositing ?)
            compute_dependencies();
        }
    }
    catch (Cool::Exception const& e)
    {
        handle_error(e.error_message(), name(), _shader_compilation_error);
        return;
    }
}

void Module_Particles::imgui_debug_menu(Cool::SetDirty_Ref set_dirty) const
{
    if (!_particle_system.has_value())
        return;

    if (ImGui::DragScalar("Particles Count", ImGuiDataType_U64, &_particles_count))
    {
        _particle_system->set_particles_count(_particles_count);
        set_dirty(dirty_flag());
    }

    if (ImGui::SliderFloat("Particle Size", &_particle_size, 0.f, 0.1f))
    {
        _particle_system->set_particle_size(_particle_size);
        set_dirty(dirty_flag());
    }
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

    _particle_system->simulation_shader().bind();
    _particle_system->simulation_shader().set_uniform("_particle_size", _particle_size);
    _particle_system->update();
}

void Module_Particles::compute_dependencies()
{
    auto const code = Cool::String::remove_comments(_shader_code);
    _dependencies.compute_dependencies(code);
}

void Module_Particles::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    Lab::DebugOptions::show_generated_shader_code([&] {
        ImGui::SeparatorText("Particle shader");
        if (Cool::ImGuiExtras::input_text_multiline("##Particles shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
        {
        }
    });

    Cool::DebugOptions::particles_debug_menu_window([&] {
        // TODO voir avec jules plus tard (push id + mettre un separator text)
        imgui_debug_menu(update_ctx.dirty_setter());
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

    shader_set_uniforms(_particle_system->simulation_shader(), in, _dependencies, *_feedback_double_buffer, *_camera_input);
    shader_send_uniforms(_particle_system->simulation_shader(), in, _nodes_graph);

    shader_set_uniforms(_particle_system->render_shader(), in, _dependencies, *_feedback_double_buffer, *_camera_input);
    shader_send_uniforms(_particle_system->render_shader(), in, _nodes_graph);

    _particle_system->render_shader().set_uniform("_particle_size", _particle_size);
    _particle_system->render();
}

} // namespace Lab
