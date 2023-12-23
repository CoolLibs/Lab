#include "shader_set_uniforms.h"
#include <variant>
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Dependencies/Input.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Nodes/Node.h"
#include "shader_boilerplate.h"

namespace Lab {

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

auto shader_set_uniforms(
    Cool::OpenGL::Shader const&             shader,
    Cool::InputProvider_Ref                 provider,
    ModuleShaderDependencyFlags const&      dependencies,
    Cool::DoubleBufferedRenderTarget const& feedback_double_buffer,
    Cool::Input<Cool::Camera> const&        camera_input,
    Cool::NodesGraph const&                 nodes_graph
) -> void
{
    shader.bind();
    shader.set_uniform("_camera2D", provider(Cool::Input_Camera2D{}));
    shader.set_uniform("_camera2D_inverse", glm::inverse(provider(Cool::Input_Camera2D{})));
    shader.set_uniform("_height", provider(Cool::Input_Height{}));
    shader.set_uniform("_aspect_ratio", provider(Cool::Input_AspectRatio{}));
    shader.set_uniform("_inverse_aspect_ratio", 1.f / provider(Cool::Input_AspectRatio{}));
    shader.set_uniform_texture("mixbox_lut", Cool::TextureLibrary_FromFile::instance().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png")->id());

    shader.set_uniform("_time", provider(Cool::Input_Time{}));
    shader.set_uniform("_delta_time", provider(Cool::Input_DeltaTime{}));

    if (dependencies.depends_on_audio_volume)
        shader.set_uniform("_audio_volume", provider(Cool::Input_Audio{}).volume());
    if (dependencies.depends_on_audio_waveform)
        shader.set_uniform_texture1D("_audio_waveform", provider(Cool::Input_Audio{}).waveform_texture().id());
    if (dependencies.depends_on_audio_spectrum)
        shader.set_uniform_texture1D("_audio_spectrum", provider(Cool::Input_Audio{}).spectrum_texture().id());

    shader.set_uniform_texture(
        "_previous_frame_texture",
        feedback_double_buffer.read_target().get().texture_id(),
        Cool::TextureSamplerDescriptor{
            .repeat_mode        = Cool::TextureRepeatMode::None,
            .interpolation_mode = glpp::Interpolation::NearestNeighbour, // Very important. If set to linear, artifacts can appear over time (very visible with the Slit Scan effect).
        }
    );
    Cool::CameraShaderU::set_uniform(shader, provider(camera_input), provider(Cool::Input_AspectRatio{}));

    nodes_graph.for_each_node<Node>([&](Node const& node) { // TODO(Nodes) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work. // TODO(Modules) Each module should store a list of its inputs, so that we can set them there
        for (auto const& value_input : node.value_inputs())
        {
            std::visit([&](auto&& value_input) {
                send_uniform(value_input, shader, provider);
            },
                       value_input);
        }
    });
}

} // namespace Lab