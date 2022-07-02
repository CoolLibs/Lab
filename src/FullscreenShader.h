#pragma once

#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/MessageConsole/MessageConsole.h>
#include <Cool/MessageConsole/MessageId.h>
#include "Dependencies/Dirty.h"

namespace Lab {

class FullscreenShader {
public:
    FullscreenShader() = default;
    explicit FullscreenShader(DirtyFlag dirty_flag)
        : _dirty_flag{dirty_flag}
    {}

    void compile(
        std::string_view      fragment_shader_source_code,
        std::string_view      shader_name,
        std::string_view      module_name,
        SetClean_Ref          set_clean,
        Cool::MessageConsole& message_console
    );

    auto dirty_flag() const -> const DirtyFlag& { return _dirty_flag; }
    auto pipeline() -> Cool::FullscreenPipeline& { return _fullscreen_pipeline; }

private:
    DirtyFlag                _dirty_flag{};
    Cool::FullscreenPipeline _fullscreen_pipeline{};
    Cool::MessageId          _compile_error_message_id{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Shader Dirty Flag", _dirty_flag)
        );
    }
};

} // namespace Lab
