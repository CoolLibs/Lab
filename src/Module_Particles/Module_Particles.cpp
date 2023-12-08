#include "Module_Particles.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Particles/ParticleSystem.h"
#include "Cool/String/String.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Nodes/Node.h"
#include "Nodes/shader_set_uniforms.h"
#include "generate_simulation_shader_code.h"

namespace Lab {
Module_Particles::Module_Particles()
{
}

Module_Particles::Module_Particles(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Nodes", dirty_flag_factory}
{
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
        _particle_system = Cool::ParticleSystem{
            _particles_count,
            Cool::ParticlesShadersCode{
                .simulation = *shader_code,
                .init       = *Cool::File::to_string(Cool::Path::root() / "res/Particles/init.comp"),
                .vertex     = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert"),
                .fragment   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
            }
        };
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("Particles Compilation Failed", e.what());
        return;
    }

    // ? TODO(Particles): compute_dependencies (parent class with Compositing ?)
    // compute_dependencies();
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

    _particle_system->update();
}

static auto contains_two_or_more(std::string_view word, std::string_view text) -> bool
{
    auto const pos = Cool::String::find_word(word, text, 0);
    if (pos == std::string_view::npos)
        return false;

    auto const pos2 = Cool::String::find_word(word, text, pos + 1);
    return pos2 != std::string_view::npos;
}

void Module_Particles::compute_dependencies()
{
    auto const code            = Cool::String::remove_comments(_shader_code);
    _depends_on_time           = contains_two_or_more("_time", _shader_code);
    _depends_on_audio_volume   = contains_two_or_more("_audio_volume", _shader_code);
    _depends_on_audio_waveform = contains_two_or_more("_audio_waveform", _shader_code);
    _depends_on_audio_spectrum = contains_two_or_more("_audio_spectrum", _shader_code);
}

void Module_Particles::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    DebugOptions::show_generated_shader_code([&] {
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
    // Render on the normal render target
    render_impl(in, update_ctx);

    // Render on the feedback texture
    _feedback_double_buffer.write_target().set_size(in.render_target_size);
    _feedback_double_buffer.write_target().render([&]() {
        render_impl(in, update_ctx);
    });
    _feedback_double_buffer.swap_buffers();
}

void Module_Particles::render_impl(RenderParams in, UpdateContext_Ref update_ctx)
{
    if (!_particle_system)
        return;

    auto const& simulation_shader = _particle_system->simulation_shader();
    auto const& render_shader     = _particle_system->render_shader();

    auto const& deps = ModuleShaderDependencyFlags{
        .depends_on_time           = _depends_on_time,
        .depends_on_audio_volume   = _depends_on_audio_volume,
        .depends_on_audio_waveform = _depends_on_audio_waveform,
        .depends_on_audio_spectrum = _depends_on_audio_spectrum,
        .feedback_double_buffer    = _feedback_double_buffer,
    };

    shader_set_uniforms(simulation_shader, in, deps);
    shader_send_uniforms(simulation_shader, in, _nodes_graph);

    shader_set_uniforms(render_shader, in, deps);
    shader_send_uniforms(render_shader, in, _nodes_graph);

    _particle_system->render();
}

} // namespace Lab
