#include "Module_Particles.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/Particles/ParticleSystem.h"
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

void Module_Particles::compile(Cool::NodesGraph const& nodes_graph, Cool::NodeId const& root_node_id, UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    // TODO(Particles)
    //
    // _shader.pipeline().reset();        // Make sure the shader will be empty if the compilation fails.
    // _shader_compilation_error.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    // _shader_code               = "";
    // _depends_on_time           = false;
    // _depends_on_particles      = false;
    // _depends_on_audio_volume   = false;
    // _depends_on_audio_waveform = false;
    // _depends_on_audio_spectrum = false;

    if (!nodes_graph.try_get_node<Node>(root_node_id))
        return; // Otherwise we will get a default UV image instead of a transparent image.

    auto const shader_code = generate_simulation_shader_code(
        nodes_graph,
        root_node_id,
        Cool::GetNodeDefinition_Ref<NodeDefinition>{update_ctx.nodes_library()},
        update_ctx.input_provider()
    );
    if (!shader_code)
    {
        // handle_error(Cool::OptionalErrorMessage{shader_code.error()}, for_testing_nodes);
        return;
    }
    _shader_code = *shader_code;

    try
    {
        _particle_system = Cool::ParticleSystem{
            _particles_count,
            Cool::ParticlesShadersCode{
                .simulation = _shader_code,
                // .simulation = *Cool::File::to_string(Cool::Path::root() / "res/Particles/simulation.comp"),
                .vertex   = *Cool::File::to_string(Cool::Path::root() / "res/Particles/vertex.vert"),
                .fragment = *Cool::File::to_string(Cool::Path::root() / "res/Particles/fragment.frag"),
            }
        };
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::error("Particles Compilation Failed", e.what());
        return;
    }

    // TODO(Particles)
    // auto const maybe_err = _shader.compile(_shader_code, update_ctx);
    //
    // handle_error(maybe_err, for_testing_nodes);
    //
    // compute_dependencies();
}

void Module_Particles::imgui_debug_menu()
{
    // if (ImGui::DragScalar("Particles Count", ImGuiDataType_U64, &_particles_count))
    //     recreate_particle_system();
    // if (ImGui::Button("Recreate Particle System"))
    //     recreate_particle_system();
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

template<typename T>
static void send_uniform(Cool::Input<T> const& input, Cool::OpenGL::Shader const& shader, Cool::InputProvider_Ref input_provider)
{
    auto const value = [&] {
        if constexpr (std::is_same_v<T, Cool::Color>)
        {
            auto const col = input_provider(input);
            switch (static_cast<Cool::ColorSpace>(input._desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color space value for {}: {}.", input.name(), input._desired_color_space)};
            }
        }
        else if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
        {
            auto const col = input_provider(input);
            switch (static_cast<Cool::ColorAndAlphaSpace>(input._desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_and_alpha_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color and alpha space value for {}: {}.", input.name(), input._desired_color_space)};
            }
        }
        else
        {
            return input_provider(input);
        }
    }();

    Cool::set_uniform(
        shader,
        valid_property_name(input.name(), input._default_variable_id.raw()),
        value
    );

    // HACK to send an error message whenever a Texture variable has an invalid path
    if constexpr (std::is_base_of_v<Cool::TextureDescriptor, T>)
    {
        input_provider.variable_registries().of<Cool::Variable<T>>().with_mutable_ref(input._default_variable_id.raw(), [&](Cool::Variable<T>& variable) {
            auto const err = Cool::get_error(value.source);
            if (err)
            {
                Cool::Log::ToUser::console().send(
                    variable.message_id,
                    Cool::Message{
                        .category = "Missing Texture",
                        .message  = err.value(),
                        .severity = Cool::MessageSeverity::Error,
                    }
                );
            }
            else
            {
                Cool::Log::ToUser::console().remove(variable.message_id);
            }
        });
    }
}

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

    _nodes_graph->for_each_node<Node>([&](Node const& node) { // TODO(Nodes) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work. // TODO(Modules) Each module should store a list of its inputs, so that we can set them there
        for (auto const& value_input : node.value_inputs())
        {
            std::visit([&](auto&& value_input) {
                send_uniform(value_input, _particle_system->simulation_shader(), in.provider);
            },
                       value_input);
        }
    });

    _particle_system->render_shader().bind();
    _particle_system->render_shader().set_uniform("_time", in.provider(Cool::Input_Time{}));
    _particle_system->render_shader().set_uniform("_delta_time", in.provider(Cool::Input_DeltaTime{}));
    _particle_system->render_shader().set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    _particle_system->render_shader().set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    _particle_system->render_shader().set_uniform("_height", in.provider(Cool::Input_Height{}));
    _particle_system->render_shader().set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    _nodes_graph->for_each_node<Node>([&](Node const& node) { // TODO(Nodes) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work. // TODO(Modules) Each module should store a list of its inputs, so that we can set them there
        for (auto const& value_input : node.value_inputs())
        {
            std::visit([&](auto&& value_input) {
                send_uniform(value_input, _particle_system->render_shader(), in.provider);
            },
                       value_input);
        }
    });

    _particle_system->render();
}

} // namespace Lab
