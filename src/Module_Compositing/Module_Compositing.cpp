#include "Module_Compositing.h"
#include <Common/make_shader_compilation_error_message.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/String/String.h>
#include <NodesGraph/Node.h>
#include <NodesGraph/NodeDefinition.h>
#include "Cool/Camera/CameraShaderU.h"
#include "Cool/ColorSpaces/ColorAndAlphaSpace.h"
#include "Cool/ColorSpaces/ColorSpace.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/StrongTypes/set_uniform.h"
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

void Module_Compositing::compile(Cool::NodesGraph const& nodes_graph, Cool::NodeId const& root_node_id, UpdateContext_Ref update_ctx, bool for_testing_nodes)
{
    _shader.pipeline().reset();        // Make sure the shader will be empty if the compilation fails.
    _shader_compilation_error.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    _shader_code               = "";
    _depends_on_time           = false;
    _depends_on_particles      = false;
    _depends_on_audio_volume   = false;
    _depends_on_audio_waveform = false;
    _depends_on_audio_spectrum = false;

    if (!nodes_graph.try_get_node<Node>(root_node_id))
        return; // Otherwise we will get a default UV image instead of a transparent image.

    auto const shader_code = generate_compositing_shader_code(
        nodes_graph,
        root_node_id,
        Cool::GetNodeDefinition_Ref<NodeDefinition>{update_ctx.nodes_library()},
        update_ctx.input_provider()
    );

    if (!shader_code)
    {
        handle_error(Cool::OptionalErrorMessage{shader_code.error()}, for_testing_nodes);
        return;
    }

    _shader_code = *shader_code;

    auto const maybe_err = _shader.compile(_shader_code, update_ctx);

    handle_error(maybe_err, for_testing_nodes);

    compute_dependencies();
}

void Module_Compositing::handle_error(Cool::OptionalErrorMessage const& maybe_err, bool for_testing_nodes) const
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
        if (Cool::ImGuiExtras::input_text_multiline("##Compositing shader code", &_shader_code, ImVec2{ImGui::GetWindowWidth() - 10, ImGui::GetWindowSize().y - 35}))
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

auto Module_Compositing::is_dirty(Cool::IsDirty_Ref check_dirty) const -> bool
{
    return Module::is_dirty(check_dirty)
           || check_dirty(_shader.dirty_flag());
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

    shader.bind();
    shader.set_uniform("_time", in.provider(Cool::Input_Time{}));
    shader.set_uniform("_camera2D", in.provider(Cool::Input_Camera2D{}));
    shader.set_uniform("_camera2D_inverse", glm::inverse(in.provider(Cool::Input_Camera2D{})));
    shader.set_uniform("_height", in.provider(Cool::Input_Height{}));
    shader.set_uniform("_aspect_ratio", in.provider(Cool::Input_AspectRatio{}));
    shader.set_uniform_texture("mixbox_lut", Cool::TextureLibrary_FromFile::instance().get(Cool::Path::root() / "res/mixbox/mixbox_lut.png")->id());
    if (_depends_on_audio_volume)
        shader.set_uniform("_audio_volume", in.provider(Cool::Input_Audio{}).volume());
    if (_depends_on_audio_waveform)
        shader.set_uniform_texture1D("_audio_waveform", in.provider(Cool::Input_Audio{}).waveform_texture().id());
    if (_depends_on_audio_spectrum)
        shader.set_uniform_texture1D("_audio_spectrum", in.provider(Cool::Input_Audio{}).spectrum_texture().id());

    shader.set_uniform_texture(
        "_particles_texture",
        in.provider(Cool::Input_ParticlesTextureId{}),
        Cool::TextureSamplerDescriptor{
            .repeat_mode        = Cool::TextureRepeatMode::None,
            .interpolation_mode = glpp::Interpolation::Linear,
        }
    );
    shader.set_uniform_texture(
        "_previous_frame_texture",
        _feedback_double_buffer.read_target().get().texture_id(),
        Cool::TextureSamplerDescriptor{
            .repeat_mode        = Cool::TextureRepeatMode::None,
            .interpolation_mode = glpp::Interpolation::NearestNeighbour, // Very important. If set to linear, artifacts can appear over time (very visible with the Slit Scan effect).
        }
    );
    // Cool::CameraShaderU::set_uniform(shader, in.provider(_camera_input), in.provider(Cool::Input_AspectRatio{})); // TODO(Particles)

    // TODO(Particles) Reintroduce this:
    // _nodes_editor.graph().for_each_node<Node>([&](Node const& node) { // TODO(Nodes) Only set it for nodes that are actually compiled in the graph. Otherwise causes problems, e.g. if a webcam node is here but unused, we still request webcam capture every frame, which forces us to rerender every frame for no reason + it does extra work.
    //     for (auto const& value_input : node.value_inputs())
    //     {
    //         std::visit([&](auto&& value_input) {
    //             send_uniform(value_input, shader, in.provider);
    //         },
    //                    value_input);
    //     }
    // });

    pipeline.draw();
}

} // namespace Lab
