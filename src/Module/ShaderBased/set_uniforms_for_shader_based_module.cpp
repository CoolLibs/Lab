#include "set_uniforms_for_shader_based_module.h"
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Exception/Exception.h"
#include "Cool/Midi/MidiManager.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"
#include "Cool/TextureSource/default_textures.h" // TODO(WebGPU) Remove
#include "Nodes/Node.h"
#include "Nodes/valid_input_name.h"
#include "system_bind_group_layout.hpp"

namespace Lab {

template<typename T>
static void set_uniform(Cool::FullscreenPipelineGLSL const& pipeline, Cool::SharedVariable<T> const& var)
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
            pipeline,
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

auto set_uniforms_for_shader_based_module(
    Cool::FullscreenPipelineGLSL const&     pipeline,
    SystemValues const&                     system_values,
    ModuleDependencies const&               depends_on,
    Cool::DoubleBufferedRenderTarget const& feedback_double_buffer,
    Cool::NodesGraph const&                 nodes_graph
) -> void
{
    pipeline.set_uniform_with_name("_camera2D_transform", system_values.camera_2D.transform_matrix());
    pipeline.set_uniform_with_name("_camera2D_view", system_values.camera_2D.view_matrix());
    pipeline.set_uniform_with_name("_height", system_values.height());
    pipeline.set_uniform_with_name("_last_midi_button_pressed", Cool::midi_manager().all_values().last_button_pressed());
    pipeline.set_uniform_with_name("_last_last_midi_button_pressed", Cool::midi_manager().all_values().last_last_button_pressed());
    pipeline.set_uniform_with_name("_time_since_last_midi_button_pressed", Cool::midi_manager().all_values().time_since_last_button_pressed().as_seconds_float());
    pipeline.set_uniform_with_name("_aspect_ratio", system_values.aspect_ratio());
    pipeline.set_uniform_with_name("_inverse_aspect_ratio", system_values.inverse_aspect_ratio());
    pipeline.set_uniform_with_name("_time", system_values.time.as_seconds_float());
    pipeline.set_uniform_with_name("_delta_time", system_values.delta_time.as_seconds_float());

    if (depends_on.audio_volume)
        pipeline.set_uniform_with_name("_audio_volume", system_values.audio_manager.get().volume());

    // TODO(WebGPU)
    // if (depends_on.audio_waveform)
    //     pipeline.set_uniform_texture1D("_audio_waveform", system_values.audio_manager.get().waveform_texture().id());
    // if (depends_on.audio_spectrum)
    //     pipeline.set_uniform_texture1D("_audio_spectrum", system_values.audio_manager.get().spectrum_texture().id());

    // pipeline.set_uniform_texture(
    //     "_previous_frame_texture",
    //     feedback_double_buffer.read_target().get().texture_id(),
    //     Cool::TextureSamplerDescriptor{
    //         .repeat_mode        = Cool::TextureRepeatMode::None,
    //         .interpolation_mode = glpp::Interpolation::NearestNeighbour, // Very important. If set to linear, artifacts can appear over time (very visible with the Slit Scan effect).
    //     }
    // );
    Cool::CameraShaderU::set_uniform(pipeline, system_values.camera_3D, system_values.aspect_ratio());

    nodes_graph.for_each_node<Node>([&](Node const& node) { // TODO(Modules) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work. // TODO(Modules) Each module should store a list of its inputs, so that we can set them there
        for (auto const& value_input : node.value_inputs())
        {
            std::visit([&](auto&& value_input) {
                set_uniform(pipeline, value_input);
            },
                       value_input);
        }
    });
}

auto make_system_bing_group() -> Cool::BindGroup
{
    // Create a sampler
    wgpu::SamplerDescriptor samplerDesc;
    samplerDesc.addressModeU  = wgpu::AddressMode::ClampToEdge;
    samplerDesc.addressModeV  = wgpu::AddressMode::ClampToEdge;
    samplerDesc.addressModeW  = wgpu::AddressMode::ClampToEdge;
    samplerDesc.magFilter     = wgpu::FilterMode::Linear;
    samplerDesc.minFilter     = wgpu::FilterMode::Linear;
    samplerDesc.mipmapFilter  = wgpu::MipmapFilterMode::Linear;
    samplerDesc.lodMinClamp   = 0.0f;
    samplerDesc.lodMaxClamp   = 1.0f;
    samplerDesc.compare       = wgpu::CompareFunction::Undefined;
    samplerDesc.maxAnisotropy = 1;
    wgpu::Sampler sampler     = Cool::webgpu_context().device.createSampler(samplerDesc);

    // Create a bind group
    std::vector<wgpu::BindGroupEntry> bindings(4, wgpu::Default);

    bindings[0].binding     = 0 /* entries[0].binding */;
    bindings[0].textureView = Cool::dummy_texture().entire_texture_view();
    bindings[1].binding     = 1 /* entries[1].binding */;
    bindings[1].sampler     = sampler;
    bindings[2].binding     = 2;
    bindings[2].textureView = Cool::TextureLibrary_Image::instance().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png")->entire_texture_view();
    bindings[3].binding     = 3;
    bindings[3].sampler     = sampler;

    // A bind group contains one or multiple bindings
    wgpu::BindGroupDescriptor bindGroupDesc{};
    bindGroupDesc.layout = system_bind_group_layout();
    // There must be as many bindings as declared in the layout!
    bindGroupDesc.entryCount = bindings.size();
    bindGroupDesc.entries    = bindings.data();
    return Cool::BindGroup{bindGroupDesc};
    // TODO(WebGPU) Don't recreate the bind group every frame?
}

} // namespace Lab