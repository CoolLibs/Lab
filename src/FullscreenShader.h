#pragma once

#include <Cool/Dependencies/Dirty.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Log/MessageConsole.h>
#include <Cool/Log/MessageId.h>
#include <Cool/Log/ToUser.h>
#include "Dependencies/UpdateContext_Ref.h"

namespace Lab {

class FullscreenShader {
public:
    FullscreenShader() = default;
    explicit FullscreenShader(Cool::DirtyFlag dirty_flag)
        : _dirty_flag{dirty_flag}
    {}

    [[nodiscard]] auto compile(
        std::string_view fragment_shader_source_code,
        std::string_view shader_name,
        std::string_view module_name,
        UpdateContext_Ref
    ) -> std::optional<Cool::Message>;

    auto dirty_flag() const -> const Cool::DirtyFlag& { return _dirty_flag; }
    auto pipeline() -> Cool::FullscreenPipeline& { return _fullscreen_pipeline; }

private:
    Cool::DirtyFlag          _dirty_flag{};
    Cool::FullscreenPipeline _fullscreen_pipeline{};

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
