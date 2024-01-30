#pragma once
#include <functional>
#include "Cool/Audio/AudioManager.h"
#include "Cool/Camera/Camera.h"

namespace Lab {

struct Input_AspectRatio {
};
struct Input_Height {
};
struct Input_Camera2D {
};
struct Input_Camera3D {
};
struct Input_Time {
};
struct Input_DeltaTime {
};
struct Input_Audio {
};

class InputProvider_Ref {
public:
    InputProvider_Ref(
        float                     render_target_aspect_ratio,
        float                     height,
        float                     time,
        float                     delta_time,
        glm::mat3 const&          camera2D,
        Cool::Camera const&       camera3D,
        Cool::AudioManager const& audio_manager
    )
        : _render_target_aspect_ratio{render_target_aspect_ratio}
        , _height{height}
        , _time{time}
        , _delta_time{delta_time}
        , _camera2D{camera2D}
        , _camera3D{camera3D}
        , _audio_manager{audio_manager}
    {}

    // TODO(Variable) stop using these types and just give normal names to methods
    float operator()(const Input_AspectRatio&) const
    {
        return _render_target_aspect_ratio;
    }

    float operator()(const Input_Height&) const
    {
        return _height;
    }

    auto operator()(const Input_Time&) const -> float
    {
        return _time;
    }

    auto operator()(const Input_DeltaTime&) const -> float
    {
        return _delta_time;
    }

    auto operator()(const Input_Audio&) const -> Cool::AudioManager const&
    {
        return _audio_manager;
    }

    auto operator()(const Input_Camera2D&) const -> glm::mat3 const&
    {
        return _camera2D;
    }
    auto operator()(const Input_Camera3D&) const -> Cool::Camera const&
    {
        return _camera3D;
    }

private:
    float                                            _render_target_aspect_ratio;
    float                                            _height;
    float                                            _time;
    float                                            _delta_time;
    glm::mat3                                        _camera2D;
    Cool::Camera                                     _camera3D;
    std::reference_wrapper<Cool::AudioManager const> _audio_manager;
};

} // namespace Lab
