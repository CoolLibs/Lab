#pragma once
#include <functional> // For std::reference_wrapper
#include "Cool/Audio/AudioManager.h"
#include "Cool/Camera/Camera.h"
#include "Cool/StrongTypes/Camera2D.h"

namespace Lab {

struct SystemValues {
    img::Size                                        render_target_size{};
    float                                            time{};
    float                                            delta_time{};
    Cool::Camera2D                                   camera_2D{};
    Cool::Camera                                     camera_3D{};
    std::reference_wrapper<Cool::AudioManager const> audio_manager;

    auto height() const -> float { return static_cast<float>(render_target_size.height()); }
    auto aspect_ratio() const -> float { return img::SizeU::aspect_ratio(render_target_size); }
    auto inverse_aspect_ratio() const -> float { return img::SizeU::inverse_aspect_ratio(render_target_size); }
    auto camera_2D_view_projection_matrix() const { return camera_2D.view_projection_matrix(inverse_aspect_ratio()); }
};

} // namespace Lab
