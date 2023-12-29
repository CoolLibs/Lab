#include "Module_Particles.h"
#include <Module/ShaderBased/shader_set_uniforms.h>
#include <imgui.h>
#include "Cool/Camera/Camera.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/Dependencies/InputProvider_Ref.h"
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
#include "glm/fwd.hpp"

namespace Lab {

Module_Particles::Module_Particles()
{
#ifdef __APPLE__
    Cool::Log::ToUser::error("Particles", "Particles are not supported on macOS for now.");
#endif
}

Module_Particles::Module_Particles(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::NodeId initilizer_id)
    : Module{"Particles", dirty_flag_factory}
    , _initializer_id(initilizer_id)
{
}

void Module_Particles::on_time_reset()
{
    if (_particle_system.has_value())
        _particle_system->reset();
}

void Module_Particles::set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, bool /* for_testing_nodes */, int dimension)
{
    if (!shader_code)
    {
        handle_error(Cool::OptionalErrorMessage{shader_code.error()}, name(), _shader_compilation_error);
        return;
    }

    _shader_code = *shader_code;
    _shader_compilation_error.clear();

    // TODO(Particles) Don't recreate the particle system every time, just change the shader but keep the current position and velocity of the particles
    try
    {
        if (_particle_system.has_value())
        {
            _particle_system->set_simulation_shader(_shader_code);
        }
        else
        {
            assert(dimension == 2 || dimension == 3);
            std::string init   = (dimension == 3 ? "#define COOLLAB_PARTICLES_3D\n" : "") + *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp");
            std::string vertex = std::string("#version 430\n") + (dimension == 3 ? "#define COOLLAB_PARTICLES_3D\n" : "") + *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert");

            _particle_system = Cool::ParticleSystem{
                dimension,
                Cool::ParticlesShadersCode{
                    .simulation = *shader_code,
                    .init       = init,
                    .vertex     = vertex,
                    .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
                },
                desired_particles_count()
            };

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

auto Module_Particles::desired_particles_count() const -> size_t
{
    static constexpr size_t default_particles_count{1000};

    if (!_nodes_graph)
        return default_particles_count;

    auto const* maybe_node = _nodes_graph->try_get_node<Node>(_initializer_id);
    if (!maybe_node)
        return default_particles_count;
    return maybe_node->particles_count().value_or(default_particles_count);
}

void Module_Particles::update_particles_count_ifn(UpdateContext_Ref update_context)
{
    if (!_particle_system)
        return;
    auto const particles_count = desired_particles_count();
    if (particles_count == _particle_system->particles_count())
        return;
    _particle_system->set_particles_count(particles_count); // TODO(History) Change through command
    update_context.set_dirty(needs_to_rerender_flag());
}

void Module_Particles::update(UpdateContext_Ref update_context)
{
    update_particles_count_ifn(update_context);
}

void Module_Particles::update_particles(Cool::InputProvider_Ref input_provider)
{
    if (!_particle_system)
        return;

#ifndef __APPLE__
    if (DebugOptions::log_when_updating_particles())
        Cool::Log::ToUser::info(name() + " Updating particles", "Particles updated");

    _particle_system->simulation_shader().bind();
    shader_set_uniforms(_particle_system->simulation_shader(), input_provider, _dependencies, *_feedback_double_buffer, *_camera_input, *_nodes_graph);
    _particle_system->update();
#endif
}

void Module_Particles::compute_dependencies()
{
    auto const code = Cool::String::remove_comments(_shader_code);
    _dependencies.compute_dependencies(code);
}

void Module_Particles::imgui_windows(Ui_Ref /* ui */, UpdateContext_Ref /* update_ctx */) const
{
}

void Module_Particles::imgui_show_generated_shader_code() const
{
    Cool::ImGuiExtras::input_text_multiline("##Particles shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35});
}

auto Module_Particles::needs_to_rerender(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::needs_to_rerender(check_dirty);
};

void Module_Particles::render(RenderParams in)
{
    if (!_particle_system)
        return;

    if (_needs_to_update_particles)
    {
        update_particles(in.provider);
        _needs_to_update_particles = false;
    }

#ifndef __APPLE__
    shader_set_uniforms(_particle_system->render_shader(), in.provider, _dependencies, *_feedback_double_buffer, *_camera_input, *_nodes_graph);

    auto const camera_2D_mat3 = glm::inverse(in.provider(Cool::Input_Camera2D{}));
    auto const camera_2D_mat4 = glm::mat4(
        glm::vec4(camera_2D_mat3[0], 0.f),
        glm::vec4(camera_2D_mat3[1], 0.f),
        glm::vec4(0.f),
        glm::vec4(camera_2D_mat3[2][0], camera_2D_mat3[2][1], 0.f, 1.f)
    );
    auto const& camera_3D = in.provider(*_camera_input);

    auto const full_camera_3D = camera_2D_mat4 * camera_3D.view_projection_matrix(1.);

    _particle_system->render_shader().set_uniform("cool_camera_view", camera_3D.view_matrix());
    if (_particle_system->dimension() == 2)
        _particle_system->render_shader().set_uniform("transform_matrix", camera_2D_mat4);
    else if (_particle_system->dimension() == 3)
        _particle_system->render_shader().set_uniform("transform_matrix", full_camera_3D);
    _particle_system->render();
#endif
}

} // namespace Lab
