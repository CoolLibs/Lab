#pragma once

#include <Cool/Dependencies/Dirty.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Log/MessageConsole.h>
#include <Cool/Log/MessageId.h>
#include <Cool/Log/OptionalErrorMessage.h>
#include <Cool/Log/ToUser.h>
#include "Dependencies/UpdateContext_Ref.h"

namespace Lab {

class FullscreenShader {
public:
    FullscreenShader() = default;
    explicit FullscreenShader(Cool::DirtyFlag const& dirty_flag)
        : _dirty_flag{dirty_flag}
    {}

    auto compile(
        std::string_view fragment_shader_source_code,
        UpdateContext_Ref
    ) -> Cool::OptionalErrorMessage;

    auto dirty_flag() const -> Cool::DirtyFlag const& { return _dirty_flag; }
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
