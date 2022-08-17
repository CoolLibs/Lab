#include "FullscreenShader.h"

namespace Lab {

auto FullscreenShader::compile(
    std::string_view  fragment_shader_source_code,
    std::string_view  shader_name,
    std::string_view  module_name,
    UpdateContext_Ref update_ctx
) -> std::optional<Cool::Message>
{
    const auto maybe_error = _fullscreen_pipeline.compile(fragment_shader_source_code, shader_name);
    update_ctx.set_clean(_dirty_flag);
    if (maybe_error)
    {
        return Cool::Message{
            .category         = std::string{module_name},
            .detailed_message = *maybe_error,
            .severity         = Cool::MessageSeverity::Error,
        };
    }
    else
    {
        return {};
    }
}

} // namespace Lab
