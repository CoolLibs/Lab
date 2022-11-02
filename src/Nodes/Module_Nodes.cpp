#include "Module_Nodes.h"
#include "Common/make_shader_compilation_error_message.h"

namespace Lab {

Module_Nodes::Module_Nodes(Cool::DirtyFlagFactory_Ref dirty_flag_factory)
    : Module{"Nodes", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
{
}

void Module_Nodes::update(UpdateContext_Ref update_ctx)
{
    if (_must_recompile)
    {
        _must_recompile = false;
        compile(update_ctx);
        update_ctx.set_dirty(dirty_flag());
    }
}

void Module_Nodes::compile(UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    _shader_code = "void main() { gl_FragColor = vec4(vec3(0.5, 0.1, 0.9), 1.); }";

    const auto maybe_err = _shader.compile(
        _shader_code,
        update_ctx
    );

    handle_error(maybe_err, for_testing_nodes);
}

void Module_Nodes::handle_error(Cool::OptionalErrorMessage const& maybe_err, bool for_testing_nodes)
{
    if (!for_testing_nodes)
    {
        maybe_err.send_error_if_any(_shader_compilation_error, [&](const std::string& msg) {
            return make_shader_compilation_error_message(name(), "Ray Marcher", msg);
        });
    }
#if DEBUG
    else
    {
        maybe_err.send_error_if_any(
            [&](const std::string& msg) {
                return make_shader_compilation_error_message("Test is0 Nodes", "Ray Marcher", msg);
            },
            Cool::Log::Debug::console()
        );
    }
#endif
}

void Module_Nodes::imgui_windows(Ui_Ref ui) const
{
    _must_recompile |= _nodes_editor.imgui_window(_nodes_library, ui.dirty_setter());
    ImGui::Begin("Nodes Debug");
    ImGui::End();
}

auto Module_Nodes::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;
    return inputs;
}

auto Module_Nodes::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty)
           || check_dirty(_shader.dirty_flag());
};

void Module_Nodes::render(RenderParams in, UpdateContext_Ref)
{
    if (!_shader.pipeline().shader())
        return;
    auto const& pipeline = _shader.pipeline();
    auto const& shader   = *pipeline.shader();

    shader.bind();
    shader.set_uniform("_time", in.provider(Cool::Input_Time{}));

    pipeline.draw();
}

} // namespace Lab
