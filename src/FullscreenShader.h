#pragma once

#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/MessageConsole/MessageConsole.h>
#include <Cool/MessageConsole/MessageId.h>
#include "Dependencies/Dirty.h"

namespace Lab {

class FullscreenShader {
public:
    void compile(
        std::string_view      fragment_shader_source_code,
        std::string_view      shader_name,
        std::string_view      module_name,
        Cool::MessageConsole& message_console
    );

public:
    DirtyFlag                _is_dirty;
    Cool::FullscreenPipeline _fullscreen_pipeline{};
    Cool::MessageId          _compile_error_message_id{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Shader Dirty Flag", _is_dirty)
        );
    }
};

} // namespace Lab
