#if DEBUG
#include "compile_all_custom_shaders.h"
#include <Cool/InputParser/InputParser.h>
#include <Cool/Path/Path.h>
#include "Common/FullscreenShader.h"
#include "Common/make_shader_compilation_error_message.h"
#include "Module_CustomShader/Module_CustomShader.h"

namespace Lab {

void compile_all_custom_shaders(
    Cool::InputProvider_Ref   input_provider,
    Cool::InputFactory_Ref    input_factory,
    Cool::InputDestructor_Ref input_destructor,
    UpdateContext_Ref         update_ctx
)
{
    auto& console = Cool::Log::Debug::console();

    const auto test_all_shaders_in = [&](std::filesystem::path folder) {
        for (const auto& entry : std::filesystem::directory_iterator(folder))
        {
            const auto file = entry.path();
            if (!entry.is_regular_file() ||
                file.extension() != ".frag")
            {
                continue;
            }

            const auto file_name = std::filesystem::canonical(file).string();
            Cool::Log::Debug::info("Test Custom Shader", file_name);
            auto       shader      = FullscreenShader{};
            const auto source_code = Cool::File::to_string(file.string());
            const auto inputs      = Cool::parse_all_inputs(source_code, Cool::DirtyFlag{}, input_factory);
            shader
                .compile(Cool::preprocess_inputs(source_code, inputs, input_provider), update_ctx)
                .send_error_if_any([&](const std::string& msg) {
                    return make_shader_compilation_error_message("Test Custom Shader", file_name, msg);
                },
                                   console);

            for (const auto& input : inputs)
            {
                input_destructor(input);
            }
        }
    };

    test_all_shaders_in(Cool::Path::root() + "/res/post-processing-examples");
    test_all_shaders_in(Cool::Path::root() + "/res/shader-demo");
    test_all_shaders_in(Cool::Path::root() + "/res/shader-examples");
}

} // namespace Lab

#endif // DEBUG