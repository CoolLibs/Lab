#include "Module_Nodes.h"
#include "Common/make_shader_compilation_error_message.h"
#include "Debug/DebugOptions.h"
#include "generate_shader_code.h"

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
#if DEBUG
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::Debug::info("Nodes", "Compiled");
#endif
        _must_recompile = false;
        compile(update_ctx);
        update_ctx.set_dirty(dirty_flag());
    }
}

void Module_Nodes::compile(UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    if (_nodes_editor.graph().nodes().is_empty())
        return;
    _shader_code = generate_shader_code(_nodes_editor.graph(), _nodes_library, _nodes_editor.graph().nodes().begin()->first);

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
            return make_shader_compilation_error_message(name(), "Shader Generator", msg);
        });
    }
#if DEBUG
    else
    {
        maybe_err.send_error_if_any(
            [&](const std::string& msg) {
                return make_shader_compilation_error_message("Test Nodes", "Ray Marcher", msg);
            },
            Cool::Log::Debug::console()
        );
    }
#endif
}

void Module_Nodes::imgui_windows(Ui_Ref) const
{
    _must_recompile |= _nodes_editor.imgui_window(_nodes_library);

    ImGui::Begin("Nodes Code");
    if (ImGui::InputTextMultiline("##Nodes shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
    {
        // _must_recompile = true;
        // ui.set_dirty()
        // _shader.compile(_shader_code, "is0 Ray Marcher", ); // TODO(JF) just set shader dirty
    }
    ImGui::End();
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
