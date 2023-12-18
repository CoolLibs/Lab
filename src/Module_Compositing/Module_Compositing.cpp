#include "Module_Compositing.h"
#include <Common/make_shader_compilation_error_message.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/String/String.h>
#include <Dependencies/UpdateContext_Ref.h>
#include <Nodes/Node.h>
#include <Nodes/NodeDefinition.h>
#include <Nodes/shader_set_uniforms.h>
#include <imgui.h>
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/StrongTypes/set_uniform.h"
#include "Module/ShaderBased/handle_error.h"
#include "generate_compositing_shader_code.h"

namespace Lab {

Module_Compositing::Module_Compositing(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Compositing", dirty_flag_factory}
{
}

void Module_Compositing::update(UpdateContext_Ref)
{
}

void Module_Compositing::reset_shader()
{
    _pipeline.reset();                 // Make sure the shader will be empty if the compilation fails.
    _shader_compilation_error.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    _shader_code = "";
    _dependencies.reset();
}

void Module_Compositing::on_time_reset()
{
    _feedback_double_buffer.clear_render_targets();
}

void Module_Compositing::set_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx)
{
    if (!shader_code)
    {
        handle_error(Cool::OptionalErrorMessage{shader_code.error()}, name(), _shader_compilation_error);
        return;
    }

    _shader_code = *shader_code;

    auto const maybe_err = _pipeline.compile(_shader_code);

    handle_error(maybe_err, name(), _shader_compilation_error);

    compute_dependencies();
}

void Module_Compositing::compute_dependencies()
{
    auto const code = Cool::String::remove_comments(_shader_code);
    _dependencies.compute_dependencies(code);
}

void Module_Compositing::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    DebugOptions::show_generated_shader_code([&] {
        ImGui::SeparatorText("Compositing shader");
        if (Cool::ImGuiExtras::input_text_multiline("##Compositing shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
        {
            const auto maybe_err = _pipeline.compile(_shader_code);
            handle_error(maybe_err, name(), _shader_compilation_error);

            ui.dirty_setter()(needs_to_rerender_flag()); // Trigger rerender
        }
    });
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

void Module_Compositing::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    // Render on the normal render target
    render_impl(in, update_ctx);

    // Render on the feedback texture
    _feedback_double_buffer.write_target().render([&]() {
        render_impl(in, update_ctx);
    });
    _feedback_double_buffer.swap_buffers();
}

void Module_Compositing::render_impl(RenderParams in, UpdateContext_Ref update_ctx)
{
    if (!_pipeline.shader())
        return;

    shader_set_uniforms(*_pipeline.shader(), in, _dependencies, _feedback_double_buffer, *_camera_input);
    shader_send_uniforms(*_pipeline.shader(), in, _nodes_graph);

    _pipeline.draw();
}

} // namespace Lab
