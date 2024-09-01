#include "Module_FeedbackLoop.hpp"
#include "Cool/Gpu/OpenGL/FullscreenPipeline.h"
#include "ModulesGraph/ModulesGraphNode.hpp"

namespace Lab {

Module_FeedbackLoop::Module_FeedbackLoop()
    : Module{"Feedback Loop"}
{
}

// TODO(FeedbackLoop) each time the module before you renders, you need to request a rerender for the next frame

void Module_FeedbackLoop::on_time_reset()
{
    // TODO(FeedbackLoop) Reset the textures
}

auto Module_FeedbackLoop::texture() const -> Cool::TextureRef
{
    return _bob ? _render_target.texture_ref() : render_target().texture_ref();
}

static auto make_copy_tex_pipeline() -> Cool::OpenGL::FullscreenPipeline
{
    auto       instance = Cool::OpenGL::FullscreenPipeline{};
    auto const res      = instance.compile(R"GLSL(
#version 410
#include "_COOL_RES_/shaders/shader-utils.glsl" 
out vec4 out_Color;
uniform sampler2D tex_to_copy;
void main()
{
    out_Color = texture(tex_to_copy, _uv);
}
    )GLSL");
    res.send_error_if_any(
        [&](std::string const& message) {
            return Cool::Message{
                .category = "Internal",
                .message  = "Failed to create shader to copy texture:\n" + message,
                .severity = Cool::MessageSeverity::Error,
            };
        },
        Cool::Log::ToUser::console()
    );

    return instance;
}

static auto copy_tex_pipeline() -> Cool::OpenGL::FullscreenPipeline&
{
    static auto instance = make_copy_tex_pipeline();
    return instance;
}

void Module_FeedbackLoop::render(DataToPassToShader const& data, std::vector<std::shared_ptr<ModulesGraphNode>> const& module_dependencies)
{
    _bob = !_bob;
    assert(module_dependencies.size() == 1);
    auto& rt = _bob ? render_target() : _render_target;
    rt.set_size(data.system_values.render_target_size);
    rt.render([&]() {
        // TODO(WebGPU) use a texture copy operation instead, it will be more efficient
        copy_tex_pipeline().shader()->bind();
        copy_tex_pipeline().shader()->set_uniform_texture("tex_to_copy", module_dependencies[0]->module->texture().id);
        glDisable(GL_BLEND);
        copy_tex_pipeline().draw();
        glEnable(GL_BLEND);
    });
}

} // namespace Lab
