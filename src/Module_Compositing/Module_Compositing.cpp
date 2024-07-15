#include "Module_Compositing.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/WebGPU/FullscreenPipelineGLSL.h"
#include "Module/ShaderBased/set_uniforms_for_shader_based_module.h"

namespace Lab {

Module_Compositing::Module_Compositing()
    : Module{"Compositing"}
{
}

void Module_Compositing::update()
{
}

void Module_Compositing::reset_shader()
{
    _pipeline.reset();            // Make sure the shader will be empty if the compilation fails.
    _shader_error_sender.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    _shader_code = "";
    _depends_on  = {};
}

void Module_Compositing::on_time_reset()
{
    _feedback_double_buffer.clear_render_targets();
}

void Module_Compositing::set_shader_code(tl::expected<std::string, std::string> const& shader_code)
{
    if (!shader_code)
    {
        log_shader_error(shader_code.error());
        return;
    }

    _shader_code = *shader_code;
    std::cout << _shader_code << '\n';

    auto entries = std::vector<wgpu::BindGroupLayoutEntry>(2, wgpu::Default);

    // TODO(WebGPU) Store the layout instead of recreating it each time we set the shader code ?
    entries[0].binding               = 0;
    entries[0].visibility            = wgpu::ShaderStage::Fragment;
    entries[0].texture.sampleType    = wgpu::TextureSampleType::Float;
    entries[0].texture.viewDimension = wgpu::TextureViewDimension::_2D;

    entries[1].binding      = 1;
    entries[1].visibility   = wgpu::ShaderStage::Fragment;
    entries[1].sampler.type = wgpu::SamplerBindingType::Filtering;

    wgpu::BindGroupLayoutDescriptor bindGroupLayoutDesc{};
    bindGroupLayoutDesc.entryCount = entries.size();
    bindGroupLayoutDesc.entries    = entries.data();

    _bind_group_layout                     = Cool::BindGroupLayout{bindGroupLayoutDesc};
    /* auto const maybe_err = */ _pipeline = Cool::make_fullscreen_pipeline_glsl({.fragment_shader_module_creation_args = {
                                                                                      .label = "TODO(WebGPU)",
                                                                                      .code  = _shader_code,
                                                                                  },
                                                                                  .extra_bind_group_layout = &*_bind_group_layout})
                                                 .value();
    // log_shader_error(maybe_err); // TODO(WebGPU)
    update_dependencies_from_shader_code(_depends_on, _shader_code);
    needs_to_rerender_flag().set_dirty();
}

void Module_Compositing::log_shader_error(Cool::OptionalErrorMessage const& maybe_err) const
{
    log_module_error(maybe_err, _shader_error_sender);
}

void Module_Compositing::imgui_windows(Ui_Ref) const
{
}

void Module_Compositing::imgui_show_generated_shader_code()
{
    if (Cool::ImGuiExtras::input_text_multiline("##Compositing shader code", &_shader_code, ImVec2{-1.f, -1.f}))
    {
        set_shader_code(_shader_code);
    }
}

void Module_Compositing::set_render_target_size(img::Size const& size)
{
    _feedback_double_buffer.write_target().set_size(size);
    _feedback_double_buffer.set_read_target_size_immediately(size);
}

void Module_Compositing::render(wgpu::RenderPassEncoder render_pass, SystemValues const& system_values)
{
    // TODO(Performance) Render only once and then copy to the _feedback_double_buffer ?
    // TODO(Performance) Only render on the _feedback_double_buffer when someone depends on it
    // Render on the normal render target
    render_impl(render_pass, system_values);

    // Render on the feedback texture
    // TODO(WebGPU)
    // _feedback_double_buffer.write_target().render([&]() {
    //     render_impl(system_values);
    // });
    _feedback_double_buffer.swap_buffers();
}

// TODO(WebGPU) Remove
static auto dummy_texture() -> Cool::Texture const&
{
    static auto instance = texture_from_pixels(img::Size{1, 1}, wgpu::TextureFormat::RGBA8Unorm, Cool::AlphaSpace::Any, std::array<uint8_t, 4>{255, 255, 0, 255});
    return instance;
}

void Module_Compositing::render_impl(wgpu::RenderPassEncoder render_pass, SystemValues const& system_values)
{
    if (!_pipeline.has_value())
        return;

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
    std::vector<wgpu::BindGroupEntry> bindings(2, wgpu::Default);

    bindings[0].binding     = 0 /* entries[0].binding */;
    bindings[0].textureView = dummy_texture().entire_texture_view();
    bindings[1].binding     = 1 /* entries[1].binding */;
    bindings[1].sampler     = sampler;

    // A bind group contains one or multiple bindings
    wgpu::BindGroupDescriptor bindGroupDesc{};
    bindGroupDesc.layout = *_bind_group_layout;
    // There must be as many bindings as declared in the layout!
    bindGroupDesc.entryCount = bindings.size();
    bindGroupDesc.entries    = bindings.data();
    _bind_group              = Cool::BindGroup{bindGroupDesc};
    // TODO(WebGPU) Don't recreate the bind group every frame

    set_uniforms_for_shader_based_module(*_pipeline, system_values, _depends_on, _feedback_double_buffer, *_nodes_graph);
    _pipeline->set_aspect_ratio_uniform(system_values.aspect_ratio());
    _pipeline->draw(render_pass, &*_bind_group);
}

} // namespace Lab
