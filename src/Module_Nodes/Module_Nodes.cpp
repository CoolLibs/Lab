#include "Module_Nodes.h"
#include <Cool/StrongTypes/set_uniform.h>
#include "Common/make_shader_compilation_error_message.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Debug/DebugOptions.h"
#include "generate_shader_code.h"
#include "imgui.h"
#include "parse_node_definition.h"

namespace Lab {

Module_Nodes::Module_Nodes(Cool::DirtyFlagFactory_Ref dirty_flag_factory)
    : Module{"Nodes", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
    , _regenerate_code_flag{dirty_flag_factory.make()}
{
}

void Module_Nodes::update(UpdateContext_Ref ctx)
{
    if (_nodes_folder_watcher.update(_nodes_library, &parse_node_definition))
        ctx.set_dirty(_regenerate_code_flag);
}

void Module_Nodes::compile(UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    if (_nodes_editor.graph().nodes().is_empty())
        return;

    auto const shader_code = generate_shader_code(
        _main_node_id,
        _nodes_editor.graph(),
        Cool::GetNodeDefinition_Ref{_nodes_library},
        update_ctx.input_provider()
    );

    if (!shader_code)
    {
        handle_error(Cool::OptionalErrorMessage{shader_code.error()}, for_testing_nodes);
        return;
    }

    _shader_code = *shader_code;

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
            return make_shader_compilation_error_message(name(), "", msg);
        });
    }
#if DEBUG
    else
    {
        maybe_err.send_error_if_any(
            [&](const std::string& msg) {
                return make_shader_compilation_error_message("Test Nodes", "", msg);
            },
            Cool::Log::Debug::console()
        );
    }
#endif
}

void Module_Nodes::imgui_windows(Ui_Ref ui) const
{
    if (_nodes_editor.imgui_window(NodesConfig{ui.input_factory(), _nodes_library, ui, _main_node_id, _shader.dirty_flag(), _regenerate_code_flag}, _nodes_library))
        ui.set_dirty(_regenerate_code_flag);

    ImGui::Begin("Nodes Code");
    if (ImGui::InputTextMultiline("##Nodes shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
    {
        // _must_recompile = true;
        // ui.set_dirty()
        // _shader.compile(_shader_code, "is0 Ray Marcher", ); // TODO(JF) just set shader dirty
    }
    ImGui::End();
    ImGui::Begin("Nodes Debug");
    if (ImGui::Button("Refresh Definitions"))
    {
        _nodes_folder_watcher.force_refresh();
    }
    ImGui::End();
}

auto Module_Nodes::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;

    std::shared_lock lock{_nodes_editor.graph().nodes().mutex()};
    for (auto const& [_, node] : _nodes_editor.graph().nodes())
    {
        for (auto const& input : node.properties())
        {
            inputs.push_back(std::visit([](auto&& input) { return Cool::AnyInputRefToConst{input}; }, input));
        }
    }

    return inputs;
}

auto Module_Nodes::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty)
           || check_dirty(_shader.dirty_flag())
           || check_dirty(_regenerate_code_flag);
};

void Module_Nodes::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    in.set_clean(_shader.dirty_flag());

    if (in.is_dirty(_regenerate_code_flag))
    {
#if DEBUG
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::Debug::info("Nodes", "Compiled");
#endif
        compile(update_ctx);
        in.set_clean(_regenerate_code_flag);
    }

    if (!_shader.pipeline().shader())
        return;

    auto const& pipeline = _shader.pipeline();
    auto const& shader   = *pipeline.shader();

    shader.bind();
    shader.set_uniform("_time", in.provider(Cool::Input_Time{}));
    shader.set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    {
        std::shared_lock lock{_nodes_editor.graph().nodes().mutex()};
        for (auto const& [_, node] : _nodes_editor.graph().nodes())
        {
            for (auto const& prop : node.properties())
            {
                std::visit([&](auto&& prop) {
                    Cool::set_uniform(
                        shader,
                        valid_property_name(prop.name(), prop._default_variable_id),
                        in.provider(prop)
                    );
                },
                           prop);
            }
        }
    }

    pipeline.draw();
}

} // namespace Lab
