#include "set_uniforms_for_shader_based_module.h"
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Exception/Exception.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Nodes/Node.h"
#include "Nodes/valid_input_name.h"

namespace Lab {

template<typename T>
static void set_uniform(Cool::OpenGL::Shader const& shader, Cool::Input<T> const& input)
{
    auto const value = [&] {
        if constexpr (std::is_same_v<T, Cool::Color>)
        {
            auto const col = input.value();
            switch (static_cast<Cool::ColorSpace>(input._desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color space value for {}: {}.", input.name(), input._desired_color_space)};
            }
        }
        else if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
        {
            auto const col = input.value();
            switch (static_cast<Cool::ColorAndAlphaSpace>(input._desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_and_alpha_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color and alpha space value for {}: {}.", input.name(), input._desired_color_space)};
            }
        }
        else
        {
            return input.value();
        }
    }();

    try
    {
        Cool::set_uniform(
            shader,
            valid_input_name(input),
            value
        );
        Cool::Log::ToUser::console().remove(input._message_id);
    }
    catch (Cool::Exception const& e)
    {
        e.error_message().send_error_if_any(
            input._message_id,
            [&](std::string const& msg) {
                return Cool::Message{
                    .category = "Invalid node parameter",
                    .message  = msg,
                    .severity = Cool::MessageSeverity::Error,
                };
            },
            Cool::Log::ToUser::console()
        );
    }

    // HACK to send an error message whenever a Texture variable has an invalid path
    if constexpr (std::is_base_of_v<Cool::TextureDescriptor, T>)
    {
        auto const err = Cool::get_error(value.source);
        if (err)
        {
            Cool::Log::ToUser::console().send(
                input._message_id,
                Cool::Message{
                    .category = "Missing Texture",
                    .message  = err.value(),
                    .severity = Cool::MessageSeverity::Error,
                }
            );
        }
        else
        {
            Cool::Log::ToUser::console().remove(input._message_id);
        }
    }
}

auto set_uniforms_for_shader_based_module(
    Cool::OpenGL::Shader const&             shader,
    SystemValues const&                     system_values,
    ModuleDependencies const&               depends_on,
    Cool::DoubleBufferedRenderTarget const& feedback_double_buffer,
    Cool::NodesGraph const&                 nodes_graph
) -> void
{
    shader.bind();
    shader.set_uniform("_camera2D", system_values.camera_2D.transform_matrix());
    shader.set_uniform("_camera2D_inverse", glm::inverse(system_values.camera_2D.view_matrix()));
    shader.set_uniform("_height", system_values.height());
    shader.set_uniform("_aspect_ratio", system_values.aspect_ratio());
    shader.set_uniform("_inverse_aspect_ratio", system_values.inverse_aspect_ratio());
    shader.set_uniform_texture("mixbox_lut", Cool::TextureLibrary_FromFile::instance().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png")->id());
    shader.set_uniform("_time", system_values.time);
    shader.set_uniform("_delta_time", system_values.delta_time);

    if (depends_on.audio_volume)
        shader.set_uniform("_audio_volume", system_values.audio_manager.get().volume());
    if (depends_on.audio_waveform)
        shader.set_uniform_texture1D("_audio_waveform", system_values.audio_manager.get().waveform_texture().id());
    if (depends_on.audio_spectrum)
        shader.set_uniform_texture1D("_audio_spectrum", system_values.audio_manager.get().spectrum_texture().id());

    shader.set_uniform_texture(
        "_previous_frame_texture",
        feedback_double_buffer.read_target().get().texture_id(),
        Cool::TextureSamplerDescriptor{
            .repeat_mode        = Cool::TextureRepeatMode::None,
            .interpolation_mode = glpp::Interpolation::NearestNeighbour, // Very important. If set to linear, artifacts can appear over time (very visible with the Slit Scan effect).
        }
    );
    Cool::CameraShaderU::set_uniform(shader, system_values.camera_3D, system_values.aspect_ratio());

    nodes_graph.for_each_node<Node>([&](Node const& node) { // TODO(Modules) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work. // TODO(Modules) Each module should store a list of its inputs, so that we can set them there
        for (auto const& value_input : node.value_inputs())
        {
            std::visit([&](auto&& value_input) {
                set_uniform(shader, value_input);
            },
                       value_input);
        }
    });
}

} // namespace Lab