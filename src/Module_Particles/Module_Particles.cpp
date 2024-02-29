#include "Module_Particles.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Module/ShaderBased/set_uniforms_for_shader_based_module.h"
#include "Nodes/Node.h"

namespace Lab {

Module_Particles::Module_Particles(Cool::NodeId const& id_of_node_storing_particles_count)
    : Module{"Particles"}
    , _id_of_node_storing_particles_count{id_of_node_storing_particles_count}
{
}

void Module_Particles::on_time_reset()
{
    if (_particle_system.has_value())
        request_particles_to_reset();
}

void Module_Particles::set_simulation_shader_code(tl::expected<std::string, std::string> const& shader_code, bool /* for_testing_nodes */, int dimension)
{
    if (!shader_code)
    {
        log_simulation_shader_error(shader_code.error());
        return;
    }

    _shader_code = *shader_code;
    _simulation_shader_error_sender.clear();

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
            _particle_system = Cool::ParticleSystem{
                dimension,
                Cool::ParticlesShadersCode{
                    .simulation = *shader_code,
                    .init       = fmt::format(
                        "{}{}",
                        dimension == 3 ? "#define COOLLAB_PARTICLES_3D\n" : "#define COOLLAB_PARTICLES_2D\n",
                        *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp")
                    ),
                    .vertex = fmt::format(
                        "#version 430\n{}{}",
                        dimension == 3 ? "#define COOLLAB_PARTICLES_3D\n" : "",
                        *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert")
                    ),
                    .fragment = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
                },
                desired_particles_count()
            };
        }
    }
    catch (Cool::Exception const& e)
    {
        log_simulation_shader_error(e.error_message());
        return;
    }
    update_dependencies_from_shader_code(_depends_on, _shader_code);
    request_particles_to_reset();
}

auto Module_Particles::desired_particles_count() const -> size_t
{
    static constexpr size_t default_particles_count{1000};

    if (!_nodes_graph)
        return default_particles_count;

    auto const* const maybe_node = _nodes_graph->try_get_node<Node>(_id_of_node_storing_particles_count);
    if (!maybe_node)
        return default_particles_count;
    return maybe_node->particles_count().value_or(default_particles_count);
}

void Module_Particles::log_simulation_shader_error(Cool::OptionalErrorMessage const& maybe_err) const
{
    log_module_error(maybe_err, _simulation_shader_error_sender);
}

void Module_Particles::update_particles_count_ifn()
{
    if (!_particle_system)
        return;
    auto const particles_count = desired_particles_count();
    if (particles_count == _particle_system->particles_count())
        return;
    _particle_system->set_particles_count(particles_count); // TODO(History) Change through command
    request_particles_to_reset();
}

void Module_Particles::request_particles_to_reset()
{
    _particle_system->reset();
    request_particles_to_update();
    _force_init_particles = true;
}

void Module_Particles::update()
{
    update_particles_count_ifn();
}

void Module_Particles::update_particles(SystemValues const& system_values)
{
    if (!_particle_system)
        return;

#if !defined(COOL_PARTICLES_DISABLED_REASON)
    if (DebugOptions::log_when_updating_particles())
        Cool::Log::ToUser::info(name(), "Updated particles");

    _particle_system->simulation_shader().bind();
    _particle_system->simulation_shader().set_uniform("_force_init_particles", _force_init_particles);
    set_uniforms_for_shader_based_module(_particle_system->simulation_shader(), system_values, _depends_on, *_feedback_double_buffer, *_nodes_graph);
    _particle_system->update();
    _force_init_particles      = false;
    _needs_to_update_particles = false;
#else
    std::ignore = system_values;
#endif
}

void Module_Particles::imgui_windows(Ui_Ref) const
{
}

void Module_Particles::imgui_show_generated_shader_code()
{
    if (!_particle_system)
        return;
    if (Cool::ImGuiExtras::input_text_multiline("##Particles simulation", &_shader_code, ImVec2{-1.f, -1.f}))
        set_simulation_shader_code(_shader_code, false, _particle_system->dimension());
}

void Module_Particles::render(SystemValues const& system_values)
{
    if (!_particle_system)
        return;

    if (_needs_to_update_particles)
        update_particles(system_values);

#if !defined(COOL_PARTICLES_DISABLED_REASON)
    set_uniforms_for_shader_based_module(_particle_system->render_shader(), system_values, _depends_on, *_feedback_double_buffer, *_nodes_graph);

    auto const view_proj_matrix_2D_mat3 = system_values.camera_2D_view_projection_matrix();
    auto const view_proj_matrix_2D_mat4 = glm::mat4{
        glm::vec4{view_proj_matrix_2D_mat3[0], 0.f},
        glm::vec4{view_proj_matrix_2D_mat3[1], 0.f},
        glm::vec4{0.f},
        glm::vec4{view_proj_matrix_2D_mat3[2][0], view_proj_matrix_2D_mat3[2][1], 0.f, view_proj_matrix_2D_mat3[2][2]}
    };

    if (_particle_system->dimension() == 2)
    {
        _particle_system->render_shader().set_uniform("view_proj_matrix", view_proj_matrix_2D_mat4);
    }
    else if (_particle_system->dimension() == 3)
    {
        _particle_system->render_shader().set_uniform("view_proj_matrix", view_proj_matrix_2D_mat4 * system_values.camera_3D.view_projection_matrix(1.f /* The aspect ratio is already taken into account in the camera 2D matrix */));
        _particle_system->render_shader().set_uniform("cool_camera_view", system_values.camera_3D.view_matrix());
    }
    _particle_system->render();
#endif
}

} // namespace Lab
