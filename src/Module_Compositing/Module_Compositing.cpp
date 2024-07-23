#include "Module_Compositing.h"
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/FullscreenPipelineGLSL.h"
#include "Module/ShaderBased/make_system_bind_group.hpp"
#include "Module/ShaderBased/make_system_bind_group_layout.hpp"
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
    _pipeline.reset();
    if (!shader_code)
    {
        log_shader_error(shader_code.error());
        return;
    }

    _shader_code = *shader_code;

    _depends_on = {};
    update_dependencies_from_shader_code(_depends_on, _shader_code); // Must be done before creating the bind group layout because it depends on it
    _bind_group_layout = make_system_bind_group_layout(_depends_on);

    auto maybe_pipeline = Cool::make_fullscreen_pipeline_glsl({.fragment_shader_module_creation_args = {
                                                                   .label = "Compositing Module fragment shader",
                                                                   .code  = _shader_code,
                                                               },
                                                               .extra_bind_group_layout = &*_bind_group_layout});
    if (maybe_pipeline.has_value())
        _pipeline = std::move(maybe_pipeline.value());
    else
        log_shader_error(maybe_pipeline.error());

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

void Module_Compositing::render_impl(wgpu::RenderPassEncoder render_pass, SystemValues const& system_values)
{
    if (!_pipeline.has_value())
        return;

    set_uniforms_for_shader_based_module(*_pipeline, system_values, _depends_on, _feedback_double_buffer, *_nodes_graph);
    _pipeline->set_aspect_ratio_uniform(system_values.aspect_ratio());
    _bind_group = make_system_bind_group(*_bind_group_layout, system_values, _depends_on); // We need to keep the bind group alive until "the end of the frame" (until the render pass that we are filling in is executed)
    render_pass.setBindGroup(1, *_bind_group, 0, nullptr);
    _pipeline->draw(render_pass);
}

} // namespace Lab
