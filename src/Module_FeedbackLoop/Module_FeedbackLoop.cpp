#include "Module_FeedbackLoop.hpp"
#include "Cool/Gpu/OpenGL/copy_tex_pipeline.hpp"
#include "Cool/Log/ToUser.h"

namespace Lab {

static auto module_id()
{
    static auto i{0};
    return i++;
}

Module_FeedbackLoop::Module_FeedbackLoop(std::string texture_name_in_shader, std::vector<std::shared_ptr<Module>> deps)
    : Module{
          fmt::format("Feedback Loop {}", module_id()),
          std::move(texture_name_in_shader),
          std::move(deps)
      }
{
    assert(dependencies().size() == 1);
}

void Module_FeedbackLoop::on_time_reset()
{
    _renders_count = 0;
}

auto Module_FeedbackLoop::texture() const -> Cool::TextureRef
{
    auto const b = _renders_count < 2 ? !_bob : _bob;
    return b ? _render_target.texture_ref() : render_target().texture_ref();
}

void Module_FeedbackLoop::render(DataToPassToShader const& data)
{
    _bob = !_bob;
    _renders_count++;
    _rerender_next_frame = Module::needs_to_rerender();
    auto& rt             = _bob ? render_target() : _render_target;
    rt.set_size(data.system_values.render_target_size);
    rt.render([&]() {
        // TODO(WebGPU) use a texture copy operation instead, it will be more efficient
        Cool::copy_tex_pipeline().shader()->bind();
        Cool::copy_tex_pipeline().shader()->set_uniform_texture("tex_to_copy", dependencies()[0]->texture().id);
        glDisable(GL_BLEND);
        Cool::copy_tex_pipeline().draw();
        glEnable(GL_BLEND);
    });
}

auto Module_FeedbackLoop::needs_to_rerender() const -> bool
{
    if (Module::needs_to_rerender() || _rerender_next_frame)
        Cool::Log::ToUser::info("bob", _rerender_next_frame ? "true" : "false");
    // TODO store the result, and use that in render() to check if we need to rerender next frame
    return Module::needs_to_rerender() || _rerender_next_frame;
}

} // namespace Lab
