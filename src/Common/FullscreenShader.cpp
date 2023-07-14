#include "FullscreenShader.h"

namespace Lab {

auto FullscreenShader::compile(
    std::string_view  fragment_shader_source_code,
    UpdateContext_Ref update_ctx
) -> Cool::OptionalErrorMessage
{
    update_ctx.set_clean(_dirty_flag);
    return _fullscreen_pipeline.compile(fragment_shader_source_code);
}
//

} // namespace Lab
