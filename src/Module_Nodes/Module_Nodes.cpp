#include "Module_Nodes.h"
#include <Cool/StrongTypes/set_uniform.h>
#include <stdexcept>
#include "Common/make_shader_compilation_error_message.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Cool/Gpu/TextureLibrary.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Cool/Nodes/NodesDefinitionUpdater.h"
#include "Cool/Variables/Variable.h"
#include "Debug/DebugOptions.h"
#include "Dependencies/Module.h"
#include "Module_Nodes/Module_Nodes.h"
#include "Module_Nodes/NodeDefinition.h"
#include "Module_Nodes/NodesConfig.h"
#include "UI/imgui_show.h"
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
    auto updater = Cool::NodesDefinitionUpdater{nodes_config(ctx.ui()), _nodes_editor.graph(), _nodes_library, &parse_node_definition, _nodes_folder_watcher.errors_map()};
    if (_nodes_folder_watcher.update(updater))
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
    else
    {
        maybe_err.send_error_if_any(
            [&](const std::string& msg) {
                return make_shader_compilation_error_message("Test Nodes", "", msg);
            },
            Cool::Log::ToUser::console()
        );
    }
}

auto Module_Nodes::nodes_config(Ui_Ref ui) const -> NodesConfig
{
    return {ui.input_factory(), _nodes_library, ui, _main_node_id, _shader.dirty_flag(), _regenerate_code_flag};
}

void Module_Nodes::imgui_windows(Ui_Ref ui) const
{
    if (_nodes_editor.imgui_window(nodes_config(ui), _nodes_library))
        ui.set_dirty(_regenerate_code_flag);

    DebugOptions::show_generated_shader_code([&] {
        ImGui::InputTextMultiline("##Nodes shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35});
    });
}

auto Module_Nodes::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;

    std::shared_lock lock{_nodes_editor.graph().nodes().mutex()};
    for (auto const& [_, node] : _nodes_editor.graph().nodes())
    {
        for (auto const& input : node.value_inputs())
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

template<typename T>
static void send_uniform(Cool::Input<T> const& input, Cool::OpenGL::Shader const& shader, Cool::InputProvider_Ref input_provider)
{
    auto const value = [&] {
        if constexpr (std::is_same_v<T, Cool::Color>)
        {
            auto const col = input_provider(input);
            switch (static_cast<Cool::ColorSpace>(input._desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color space value for {}: {}.", input.name(), input._desired_color_space)};
            }
        }
        else if constexpr (std::is_same_v<T, Cool::ColorAndAlpha>)
        {
            auto const col = input_provider(input);
            switch (static_cast<Cool::ColorAndAlphaSpace>(input._desired_color_space))
            {
#include "Cool/ColorSpaces/generated/convert_col_and_alpha_as.inl"
            default:
                throw std::runtime_error{fmt::format("Unknown color and alpha space value for {}: {}.", input.name(), input._desired_color_space)};
            }
        }
        else
        {
            return input_provider(input);
        }
    }();

    Cool::set_uniform(
        shader,
        valid_property_name(input.name(), input._default_variable_id.raw()),
        value
    );

    // HACK to send an error message whenever a Texture variable has an invalid path
    if constexpr (std::is_same_v<T, Cool::TextureInfo>)
    {
        input_provider.variable_registries().of<Cool::Variable<T>>().with_mutable_ref(input._default_variable_id.raw(), [&](Cool::Variable<T>& variable) {
            auto const err = Cool::TextureLibrary::instance().error_from(value.absolute_path);
            if (err)
            {
                Cool::Log::ToUser::console().send(
                    variable.message_id,
                    Cool::Message{
                        .category = "Load Image",
                        .message  = fmt::format("Failed to load {}:\n{}", value.absolute_path, *err),
                        .severity = Cool::MessageSeverity::Error,
                    }
                );
            }
            else
            {
                Cool::Log::ToUser::console().remove(variable.message_id);
            }
        });
    }
}

void Module_Nodes::render(RenderParams in, UpdateContext_Ref update_ctx)
{
    in.set_clean(_shader.dirty_flag());

    if (in.is_dirty(_regenerate_code_flag))
    {
        if (DebugOptions::log_when_compiling_nodes())
            Cool::Log::ToUser::info("Nodes", "Compiled");
        compile(update_ctx);
        in.set_clean(_regenerate_code_flag);
    }

    if (!_shader.pipeline().shader())
        return;

    auto const& pipeline = _shader.pipeline();
    auto const& shader   = *pipeline.shader();

    shader.bind();
    shader.set_uniform("_time", in.provider(Cool::Input_Time{}));
    shader.set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    shader.set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    shader.set_uniform("_height", in.provider(Cool::Input_Height{}));
    shader.set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));

    {
        std::shared_lock lock{_nodes_editor.graph().nodes().mutex()};
        for (auto const& [_, node] : _nodes_editor.graph().nodes())
        {
            for (auto const& prop : node.value_inputs())
            {
                std::visit([&](auto&& prop) {
                    send_uniform(prop, shader, in.provider);
                },
                           prop);
            }
        }
    }

    pipeline.draw();
}

void Module_Nodes::debug_show_nodes_and_links_registries_windows(Ui_Ref ui) const
{
    ui.window({.name = "Nodes Registry"}, [&]() {
        imgui_show(_nodes_editor.graph().nodes());
    });
    ui.window({.name = "Links Registry"}, [&]() {
        imgui_show(_nodes_editor.graph().links());
    });
}

} // namespace Lab
