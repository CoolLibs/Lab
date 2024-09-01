#include "Module_FeedbackLoop.hpp"
#include "Cool/Gpu/OpenGL/copy_tex_pipeline.hpp"
#include "ModulesGraph/ModulesGraphNode.hpp"

namespace Lab {

static auto module_id()
{
    static auto i{0};
    return i++;
}

Module_FeedbackLoop::Module_FeedbackLoop()
    : Module{fmt::format("Feedback Loop {}", module_id())}
{
}

// TODO(FeedbackLoop) each time the module before you renders, you need to request a rerender for the next frame

void Module_FeedbackLoop::on_time_reset()
{
    _renders_count = 0;
}

auto Module_FeedbackLoop::texture() const -> Cool::TextureRef
{
    auto const b = _renders_count < 2 ? !_bob : _bob;
    return b ? _render_target.texture_ref() : render_target().texture_ref();
}

void Module_FeedbackLoop::render(DataToPassToShader const& data, std::vector<std::shared_ptr<ModulesGraphNode>> const& module_dependencies)
{
    _bob = !_bob;
    _renders_count++;
    assert(module_dependencies.size() == 1);
    auto& rt = _bob ? render_target() : _render_target;
    rt.set_size(data.system_values.render_target_size);
    rt.render([&]() {
        // TODO(WebGPU) use a texture copy operation instead, it will be more efficient
        Cool::copy_tex_pipeline().shader()->bind();
        Cool::copy_tex_pipeline().shader()->set_uniform_texture("tex_to_copy", module_dependencies[0]->module->texture().id);
        glDisable(GL_BLEND);
        Cool::copy_tex_pipeline().draw();
        glEnable(GL_BLEND);
    });
}

} // namespace Lab
