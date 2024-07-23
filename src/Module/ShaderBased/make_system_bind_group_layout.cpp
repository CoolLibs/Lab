#include "make_system_bind_group_layout.hpp"
#include "Cool/WebGPU/BindGroupLayoutBuilder.hpp"

namespace Lab {

auto make_system_bind_group_layout(ModuleDependencies const& depends_on) -> Cool::BindGroupLayout
{
    auto builder = Cool::BindGroupLayoutBuilder{wgpu::ShaderStage::Fragment}
                       .read_texture_2D(0) // _previous_frame_texture
                       .sampler(1)         // _previous_frame_texture_sampler
                       .read_texture_2D(2) // mixbox_lut
                       .sampler(3);        // mixbox_lut_sampler

    if (depends_on.audio_spectrum)
        builder.read_buffer(4); // _audio_spectrum

    if (depends_on.audio_waveform)
        builder.read_buffer(5); // _audio_waveform

    return builder.build();
}

} // namespace Lab