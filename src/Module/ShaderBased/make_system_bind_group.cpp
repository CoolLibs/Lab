#include "make_system_bind_group.hpp"
#include "Cool/TextureSource/TextureLibrary_Image.hpp"
#include "Cool/TextureSource/default_textures.h" // TODO(WebGPU) Remove
#include "Cool/WebGPU/BindGroupBuilder.hpp"

namespace Lab {

auto make_system_bind_group(Cool::BindGroupLayout const& layout, SystemValues const& values, ModuleDependencies const& depends_on) -> Cool::BindGroup
{
    // TODO(WebGPU) Don't recreate the bind group every frame?
    auto builder = Cool::BindGroupBuilder{}
                       .read_texture(0, Cool::dummy_texture())
                       .sampler(1, {
                                       .repeat_mode        = Cool::RepeatMode::None,
                                       .interpolation_mode = Cool::InterpolationMode::NearestNeighbour, // Very important. If set to linear, artifacts can appear over time (very visible with the Slit Scan effect).
                                   })
                       .read_texture(2, *Cool::texture_library_image().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png"))
                       .sampler(3, {
                                       .repeat_mode        = Cool::RepeatMode::Clamp,
                                       .interpolation_mode = Cool::InterpolationMode::Linear,
                                   });

    if (depends_on.audio_spectrum)
        builder.buffer(4, values.audio_manager.get().spectrum_buffer());

    if (depends_on.audio_waveform)
        builder.buffer(5, values.audio_manager.get().waveform_buffer());

    return builder.build(layout);
}

} // namespace Lab