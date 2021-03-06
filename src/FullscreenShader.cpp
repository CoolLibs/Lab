#include "FullscreenShader.h"

namespace Lab {

void FullscreenShader::compile(
    std::string_view  fragment_shader_source_code,
    std::string_view  shader_name,
    std::string_view  module_name,
    UpdateContext_Ref update_ctx
)
{
    const auto maybe_error = _fullscreen_pipeline.compile(fragment_shader_source_code, shader_name);
    if (maybe_error)
    {
        update_ctx.message_console().send(
            _compile_error_message_id,
            Cool::MessageV2{
                .category         = std::string{module_name},
                .detailed_message = *maybe_error,
                .severity         = Cool::MessageSeverity::Error,
            }
        );
    }
    else
    {
        update_ctx.message_console().clear(_compile_error_message_id);
    }
    update_ctx.set_clean(_dirty_flag);
}

} // namespace Lab
