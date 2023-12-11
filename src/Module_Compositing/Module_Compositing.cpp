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
    : Module{"Nodes", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
{
}

void Module_Compositing::update(UpdateContext_Ref)
{
}

void Module_Compositing::reset()
{
    _shader.pipeline().reset();        // Make sure the shader will be empty if the compilation fails.
    _shader_compilation_error.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    _shader_code               = "";
    _depends_on_time           = false;
    _depends_on_particles      = false;
    _depends_on_audio_volume   = false;
    _depends_on_audio_waveform = false;
    _depends_on_audio_spectrum = false;
}

void Module_Compositing::set_shader_code(tl::expected<std::string, std::string> const& shader_code, UpdateContext_Ref update_ctx)
{
    if (!shader_code)
    {
        handle_error(Cool::OptionalErrorMessage{shader_code.error()}, name(), _shader_compilation_error);
        return;
    }

    _shader_code = *shader_code;

    auto const maybe_err = _shader.compile(_shader_code, update_ctx);

    handle_error(maybe_err, name(), _shader_compilation_error);

    compute_dependencies();
}

static auto contains_two_or_more(std::string_view word, std::string_view text) -> bool
{
    auto const pos = Cool::String::find_word(word, text, 0);
    if (pos == std::string_view::npos)
        return false;

    auto const pos2 = Cool::String::find_word(word, text, pos + 1);
    return pos2 != std::string_view::npos;
}

void Module_Compositing::compute_dependencies()
{
    auto const code            = Cool::String::remove_comments(_shader_code);
    _depends_on_time           = contains_two_or_more("_time", _shader_code);
    _depends_on_particles      = contains_two_or_more("_particles_texture", _shader_code);
    _depends_on_audio_volume   = contains_two_or_more("_audio_volume", _shader_code);
    _depends_on_audio_waveform = contains_two_or_more("_audio_waveform", _shader_code);
    _depends_on_audio_spectrum = contains_two_or_more("_audio_spectrum", _shader_code);
}

void Module_Compositing::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    DebugOptions::show_generated_shader_code([&] {
        ImGui::SeparatorText("Compositing shader");
        if (Cool::ImGuiExtras::input_text_multiline("##Compositing shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
        {
            const auto maybe_err = _shader.compile(
                _shader_code,
                update_ctx
            );
            handle_error(maybe_err, name(), _shader_compilation_error);

            ui.dirty_setter()(dirty_flag()); // Trigger rerender
        }
    });
}

auto Module_Compositing::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty)
           || check_dirty(_shader.dirty_flag());
};

void Module_Compositing::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    // Render on the normal render target
    render_impl(in, update_ctx);

    // Render on the feedback texture
    _feedback_double_buffer.write_target().set_size(in.render_target_size);
    _feedback_double_buffer.write_target().render([&]() {
        render_impl(in, update_ctx);
    });
    _feedback_double_buffer.swap_buffers();
}

void Module_Compositing::render_impl(RenderParams in, UpdateContext_Ref update_ctx)
{
    in.set_clean(_shader.dirty_flag());

    if (!_shader.pipeline().shader())
        return;

    auto const& pipeline = _shader.pipeline();
    auto const& shader   = *pipeline.shader();

    ModuleShaderDependencyFlags deps{
        _depends_on_time,
        _depends_on_audio_volume,
        _depends_on_audio_waveform,
        _depends_on_audio_spectrum,
    };

    shader_set_uniforms(shader, in, deps, _feedback_double_buffer);
    shader_send_uniforms(shader, in, _nodes_graph);

    pipeline.draw();
}

} // namespace Lab
