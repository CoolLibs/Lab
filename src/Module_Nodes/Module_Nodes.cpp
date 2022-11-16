#include "Module_Nodes.h"
#include <Cool/StrongTypes/set_uniform.h>
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
    auto const shader_code = generate_shader_code(
        _nodes_editor.graph(),
        Cool::GetNodeDefinition_Ref{_nodes_library},
        _main_node_id,
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
    _must_recompile |= _nodes_editor.imgui_window(NodesConfig{ui.input_factory(), ui, _main_node_id, Cool::DirtyFlag{}, Cool::DirtyFlag{}}, _nodes_library); // TODO(JF) Use an actual dirty flag stored in the Module_Nodes class

    ImGui::Begin("Nodes Code");
    if (ImGui::InputTextMultiline("##Nodes shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
    {
        // _must_recompile = true;
        // ui.set_dirty()
        // _shader.compile(_shader_code, "is0 Ray Marcher", ); // TODO(JF) just set shader dirty
    }
    ImGui::End();
    ImGui::Begin("Nodes Debug");
    {
        std::shared_lock lock{_nodes_editor.graph().nodes().mutex()};
        const auto       main_node = _nodes_editor.graph().nodes().get(_main_node_id);
        if (ImGui::BeginCombo("Main Node ID", (main_node ? main_node->definition_name() : ""s).c_str()))
        {
            for (auto const& [id, node] : _nodes_editor.graph().nodes())
            {
                const bool is_selected = id == _main_node_id;
                if (ImGui::Selectable(node.definition_name().c_str(), is_selected))
                {
                    _main_node_id   = id;
                    _must_recompile = true;
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
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
