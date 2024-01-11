#include "Module_Compositing.h"
#include <Module/ShaderBased/shader_set_uniforms.h>

namespace Lab {

Module_Compositing::Module_Compositing(Cool::DirtyFlagFactory_Ref dirty_flag_factory)
    : Module{"Compositing", dirty_flag_factory}
{
}

void Module_Compositing::update(UpdateContext_Ref)
{
}

void Module_Compositing::reset_shader()
{
    _pipeline.reset();            // Make sure the shader will be empty if the compilation fails.
    _shader_error_sender.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    _shader_code = "";
    _dependencies.reset();
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

    _shader_code         = *shader_code;
    auto const maybe_err = _pipeline.compile(_shader_code);
    log_shader_error(maybe_err);
    _dependencies.compute_dependencies(_shader_code);
}

void Module_Compositing::log_shader_error(Cool::OptionalErrorMessage const& maybe_err) const
{
    log_module_error(maybe_err, _shader_error_sender);
}

void Module_Compositing::imgui_windows(Ui_Ref /* ui */, UpdateContext_Ref /* update_ctx */) const
{
}

void Module_Compositing::imgui_show_generated_shader_code(Ui_Ref ui)
{
    if (Cool::ImGuiExtras::input_text_multiline("##Compositing shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
    {
        set_shader_code(_shader_code);
        ui.dirty_setter()(needs_to_rerender_flag());
    }
}

auto Module_Compositing::needs_to_rerender(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::needs_to_rerender(check_dirty);
};

void Module_Compositing::set_render_target_size(img::Size const& size)
{
    _feedback_double_buffer.write_target().set_size(size);
    _feedback_double_buffer.set_read_target_size_immediately(size);
}

void Module_Compositing::render(RenderParams in)
{
    // Render on the normal render target
    render_impl(in);

    // Render on the feedback texture
    _feedback_double_buffer.write_target().render([&]() {
        render_impl(in);
    });
    _feedback_double_buffer.swap_buffers();
}

void Module_Compositing::render_impl(RenderParams in)
{
    if (!_pipeline.shader())
        return;

    shader_set_uniforms(*_pipeline.shader(), in.provider, _dependencies, _feedback_double_buffer, *_camera_input, *_nodes_graph);
    _pipeline.draw();
}

} // namespace Lab
