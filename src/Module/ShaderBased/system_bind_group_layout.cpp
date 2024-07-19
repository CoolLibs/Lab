#include "system_bind_group_layout.hpp"

namespace Lab {

auto system_bind_group_layout() -> Cool::BindGroupLayout const&
{
    static auto instance = Cool::BindGroupLayoutBuilder{wgpu::ShaderStage::Fragment}
                               .read_texture_2D(0) // _previous_frame_texture
                               .sampler(1)         // _previous_frame_texture_sampler
                               .read_texture_2D(2) // mixbox_lut
                               .sampler(3)         // mixbox_lut_sampler
                               .build();
    return instance;
}

} // namespace Lab