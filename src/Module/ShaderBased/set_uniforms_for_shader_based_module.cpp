#include "set_uniforms_for_shader_based_module.hpp"
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Exception/Exception.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"
#include "Nodes/Node.h"
#include "Nodes/valid_input_name.h"

namespace Lab {

template<typename T>
static void set_uniform(Cool::OpenGL::Shader const& shader, Cool::SharedVariable<T> const& var)
{
    auto const value = [&] {
        if constexpr (std::is_same_v<T, Cool::Color>)
        {
            auto const col = var.value();
            switch (static_cast<Cool::ColorSpace>(var.get_ref().desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color space value for {}: {}.", var.name(), var.get_ref().desired_color_space)};
            }
        }
        else if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
        {
            auto const col = var.value();
            switch (static_cast<Cool::ColorAndAlphaSpace>(var.get_ref().desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_and_alpha_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color and alpha space value for {}: {}.", var.name(), var.get_ref().desired_color_space)};
            }
        }
        else
        {
            return var.value();
        }
    }();

    try
    {
        Cool::set_uniform(
            shader,
            valid_input_name(var),
            value
        );
        Cool::Log::ToUser::console().remove(var.message_id());
    }
    catch (Cool::Exception const& e)
    {
        e.error_message().send_error_if_any(
            var.message_id(),
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
                var.message_id(),
                Cool::Message{
                    .category = "Missing Texture",
                    .message  = err.value(),
                    .severity = Cool::MessageSeverity::Error,
                }
            );
        }
        else
        {
            Cool::Log::ToUser::console().remove(var.message_id());
        }
    }
}

void set_uniforms_for_shader_based_module(
    Cool::OpenGL::Shader const& shader,
    ModuleDependencies const&   depends_on,
    DataToPassToShader const&   data
)
{
    shader.bind();
    shader.set_uniform("_camera2D_transform", data.system_values.camera_2D.transform_matrix());
    shader.set_uniform("_camera2D_view", data.system_values.camera_2D.view_matrix());
    shader.set_uniform("_height", data.system_values.height());
    shader.set_uniform("_last_midi_button_pressed", Cool::midi_manager().all_values().last_button_pressed());
    shader.set_uniform("_last_last_midi_button_pressed", Cool::midi_manager().all_values().last_last_button_pressed());
    shader.set_uniform("_time_since_last_midi_button_pressed", Cool::midi_manager().all_values().time_since_last_button_pressed().as_seconds_float());
    shader.set_uniform("_aspect_ratio", data.system_values.aspect_ratio());
    shader.set_uniform("_inverse_aspect_ratio", data.system_values.inverse_aspect_ratio());
    shader.set_uniform_texture("mixbox_lut", Cool::TextureLibrary_Image::instance().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png")->id());
    shader.set_uniform("_time", data.system_values.time.as_seconds_float());
    shader.set_uniform("_delta_time", data.system_values.delta_time.as_seconds_float());

    if (depends_on.audio_volume)
        shader.set_uniform("_audio_volume", data.system_values.audio_manager.get().volume());
    if (depends_on.audio_waveform)
        shader.set_uniform_texture1D("_audio_waveform", data.system_values.audio_manager.get().waveform_texture().id());
    if (depends_on.audio_spectrum)
        shader.set_uniform_texture1D("_audio_spectrum", data.system_values.audio_manager.get().spectrum_texture().id());

    Cool::CameraShaderU::set_uniform(shader, data.system_values.camera_3D, data.system_values.aspect_ratio());

    data.nodes_graph.for_each_node<Node>([&](Node const& node) { // TODO(Modules) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work. // TODO(Modules) Each module should store a list of its inputs, so that we can set them there
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