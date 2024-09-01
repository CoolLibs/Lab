#include "Module_Compositing.h"
#include "Module/ShaderBased/set_uniforms_for_shader_based_module.hpp"

namespace Lab {

static auto module_id()
{
    static auto i{0};
    return i++;
}

Module_Compositing::Module_Compositing(std::string texture_name_in_shader, std::vector<std::shared_ptr<Module>> dependencies)
    : Module{
          fmt::format("Compositing {}", module_id()),
          std::move(texture_name_in_shader),
          std::move(dependencies)
      }
{
}

void Module_Compositing::update()
{
}

void Module_Compositing::reset_shader()
{
    _pipeline.reset();            // Make sure the shader will be empty if the compilation fails.
    _shader_error_sender.clear(); // Make sure the error is removed if for some reason we don't compile the code (e.g. when there is no main node).
    _shader_code = "";
    _depends_on  = {};
}

void Module_Compositing::set_shader_code(tl::expected<std::string, std::string> const& shader_code)
{
    if (!shader_code)
    {
        log_shader_error(shader_code.error());
        return;
    }

    _shader_code         = *shader_code;
    auto const maybe_err = _pipeline.compile(_shader_code);
    log_shader_error(maybe_err);
    _depends_on = {};
    update_dependencies_from_shader_code(_depends_on, _shader_code);
    needs_to_rerender_flag().set_dirty();
}

void Module_Compositing::log_shader_error(Cool::OptionalErrorMessage const& maybe_err) const
{
    log_module_error(maybe_err, _shader_error_sender);
}

void Module_Compositing::imgui_generated_shader_code_tab()
{
    if (ImGui::BeginTabItem(name().c_str()))
    {
        if (Cool::ImGuiExtras::input_text_multiline("##Compositing shader code", &_shader_code, ImVec2{-1.f, -1.f}))
            set_shader_code(_shader_code);
        ImGui::EndTabItem();
    }
}

void Module_Compositing::render(DataToPassToShader const& data)
{
    if (!_pipeline.shader())
        return;

    render_target().set_size(data.system_values.render_target_size);
    render_target().render([&]() {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
        set_uniforms_for_shader_based_module(*_pipeline.shader(), _depends_on, data, dependencies());
        _pipeline.draw();
    });
}

} // namespace Lab
