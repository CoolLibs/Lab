#include "Module_Nodes.h"
#include <Commands/Command_FinishedEditingVariable.h>
#include <Cool/StrongTypes/set_uniform.h>
#include <Module_Nodes/Module_Nodes.h>
#include <stdexcept>
#include <type_traits>
#include "Common/make_shader_compilation_error_message.h"
#include "Cool/Camera/Camera.h"
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Dependencies/Input.h"
#include "Cool/Dependencies/InputDefinition.h"
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Cool/Gpu/Texture.h"
#include "Cool/Gpu/TextureDescriptor.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/Gpu/TextureSamplerDescriptor.h"
#include "Cool/Gpu/TextureSource.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Nodes/GetNodeCategoryConfig.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Cool/Nodes/NodesConfig.h"
#include "Cool/Nodes/NodesDefinitionUpdater.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Variables/Variable.h"
#include "Debug/DebugOptions.h"
#include "Dependencies/Module.h"
#include "Module_Nodes/Module_Nodes.h"
#include "Module_Nodes/NodeDefinition.h"
#include "Module_Nodes/NodesConfig.h"
#include "NodesCategoryConfig.h"
#include "UI/imgui_show.h"
#include "generate_shader_code.h"
#include "imgui.h"

namespace Lab {

Module_Nodes::Module_Nodes(Cool::DirtyFlagFactory_Ref dirty_flag_factory, Cool::InputFactory_Ref input_factory)
    : Module{"Nodes", dirty_flag_factory}
    , _shader{dirty_flag_factory.make()}
    , _regenerate_code_flag{dirty_flag_factory.make()}
    , _camera_input{input_factory.make<Cool::Camera>(Cool::InputDefinition<Cool::Camera>{.name = "Camera"}, dirty_flag())}
{
}

void Module_Nodes::update(UpdateContext_Ref)
{
}

void Module_Nodes::compile(UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    _shader.pipeline().reset();        // Make sure the shader will be empty if the compilation fails.
    _shader_compilation_error.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).

    if (!_nodes_editor.graph().try_get_node<Node>(_main_node_id))
        return; // Otherwise we will get a default UV image instead of a transparent image.

    auto const shader_code = generate_shader_code(
        _main_node_id,
        _nodes_editor.graph(),
        Cool::GetNodeDefinition_Ref<NodeDefinition>{update_ctx.nodes_library()},
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

void Module_Nodes::handle_error(Cool::OptionalErrorMessage const& maybe_err, bool for_testing_nodes) const
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

auto Module_Nodes::nodes_config(Ui_Ref ui, Cool::NodesLibrary& nodes_library) const -> NodesConfig
{
    return NodesConfig{
        ui.input_factory(),
        Cool::GetNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetMutableNodeDefinition_Ref<NodeDefinition>{nodes_library},
        Cool::GetNodeCategoryConfig_Ref{nodes_library},
        ui,
        _main_node_id,
        _node_we_might_want_to_restore_as_main_node_id,
        _shader.dirty_flag(),
        _regenerate_code_flag,
        _nodes_editor.graph(),
    };
}

void Module_Nodes::imgui_windows(Ui_Ref ui, UpdateContext_Ref update_ctx) const
{
    {
        auto cfg = Cool::NodesConfig{nodes_config(ui, update_ctx.nodes_library())};
        if (_nodes_editor.imgui_windows(cfg, update_ctx.nodes_library()))
            ui.set_dirty(_regenerate_code_flag);
    }

    DebugOptions::show_generated_shader_code([&] {
        if (Cool::ImGuiExtras::input_text_multiline("##Nodes shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
        {
            const auto maybe_err = _shader.compile(
                _shader_code,
                update_ctx
            );
            handle_error(maybe_err, false);

            ui.dirty_setter()(dirty_flag()); // Trigger rerender
        }
    });
}

static auto make_gizmo(Cool::Input<Cool::Point2D> const& input, UpdateContext_Ref ctx) -> Cool::Gizmo_Point2D
{
    auto const cam_transform = ctx.input_provider()(Cool::Input_Camera2D{});
    auto const id            = input._default_variable_id.raw();
    return Cool::Gizmo_Point2D{
        .get_position = [=]() {
            auto const var = ctx.input_provider().variable_registries().get(id);
            if (!var)
                return Cool::ViewCoordinates{0.f};
            return Cool::ViewCoordinates{glm::vec2{glm::inverse(cam_transform) * glm::vec3{var->value().value, 1.f}}};
            //
        },
        .set_position = [=](Cool::ViewCoordinates pos) {
            //
            auto const world_pos = glm::vec2{cam_transform * glm::vec3{pos, 1.f}};
            ctx.ui().command_executor().execute(
                Command_SetVariable<Cool::Point2D>{.id = id, .value = Cool::Point2D{world_pos}}
            );
            //
        },
        .on_drag_stop = [=]() {
            //
            ctx.ui().command_executor().execute(
                Command_FinishedEditingVariable{}
            );
            //
        },
        .id = id,
    };
}

void Module_Nodes::submit_gizmos(Cool::GizmoManager& gizmos, UpdateContext_Ref ctx)
{
    _nodes_editor.for_each_selected_node([&](Cool::Node const& node) {
        for (auto const& input : node.downcast<Node>().value_inputs())
        {
            if (auto const* point_2D_input = std::get_if<Cool::Input<Cool::Point2D>>(&input))
            {
                gizmos.push(make_gizmo(*point_2D_input, ctx));
            }
        }
    });
}

auto Module_Nodes::all_inputs() const -> Cool::AllInputRefsToConst
{
    Cool::AllInputRefsToConst inputs;

    inputs.push_back(Cool::AnyInputRefToConst{_camera_input});

    _nodes_editor.graph().for_each_node<Node>([&](Node const& node) {
        for (auto const& input : node.value_inputs())
        {
            inputs.push_back(std::visit([](auto&& input) { return Cool::AnyInputRefToConst{input}; }, input));
        }
    });

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
    if constexpr (std::is_base_of_v<Cool::TextureDescriptor, T>)
    {
        input_provider.variable_registries().of<Cool::Variable<T>>().with_mutable_ref(input._default_variable_id.raw(), [&](Cool::Variable<T>& variable) {
            auto const err = Cool::get_error(value.source);
            if (err)
            {
                Cool::Log::ToUser::console().send(
                    variable.message_id,
                    Cool::Message{
                        .category = "Missing Texture",
                        .message  = err.value(),
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
    // Render on the normal render target
    render_impl(in, update_ctx);

    // Render on the feedback texture
    _feedback_double_buffer.write_target().set_size(in.render_target_size);
    _feedback_double_buffer.write_target().render([&]() {
        render_impl(in, update_ctx);
    });
    _feedback_double_buffer.swap_buffers();
}

void Module_Nodes::render_impl(RenderParams in, UpdateContext_Ref update_ctx)
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
    shader.set_uniform_texture("mixbox_lut", Cool::TextureLibrary_FromFile::instance().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png")->id());

    shader.set_uniform_texture(
        "_previous_frame_texture",
        _feedback_double_buffer.read_target().get().texture_id(),
        Cool::TextureSamplerDescriptor{
            .repeat_mode        = Cool::TextureRepeatMode::None,
            .interpolation_mode = glpp::Interpolation::NearestNeighbour, // Very important. If set to linear, artifacts can appear over time (very visible with the Slit Scan effect).
        }
    );
    Cool::CameraShaderU::set_uniform(shader, in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{}));

    _nodes_editor.graph().for_each_node<Node>([&](Node const& node) { // TODO(Nodes) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work.
        for (auto const& value_input : node.value_inputs())
        {
            std::visit([&](auto&& value_input) {
                send_uniform(value_input, shader, in.provider);
            },
                       value_input);
        }
    });

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
