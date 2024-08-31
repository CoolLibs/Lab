#include "Module_Compositing.h"
#include "Module/ShaderBased/set_uniforms_for_shader_based_module.hpp"

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

    _shader_code         = *shader_code;
    auto const maybe_err = _pipeline.compile(_shader_code);
    log_shader_error(maybe_err);
    _depends_on = {};
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

void Module_Compositing::render(DataToPassToShader const& data)
{
    // TODO(Performance) Render only once and then copy to the _feedback_double_buffer ?
    // TODO(Performance) Only render on the _feedback_double_buffer when someone depends on it
    // Render on the normal render target
    render_impl(data);

    // Render on the feedback texture
    _feedback_double_buffer.write_target().render([&]() {
        render_impl(data);
    });
    _feedback_double_buffer.swap_buffers();
}

void Module_Compositing::render_impl(DataToPassToShader const& data)
{
    if (!_pipeline.shader())
        return;

    set_uniforms_for_shader_based_module(*_pipeline.shader(), _depends_on, data);
    _pipeline.draw();
}

} // namespace Lab
