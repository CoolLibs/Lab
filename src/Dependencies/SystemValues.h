#pragma once
#include <functional> // For std::reference_wrapper
#include "Cool/Audio/AudioManager.h"
#include "Cool/Camera/Camera.h"

namespace Lab {

struct SystemValues {
    img::Size                                        render_target_size{};
    float                                            time{};
    float                                            delta_time{};
    glm::mat3                                        camera2D{}; // TODO(Variables) Pass full Camera2D
    Cool::Camera                                     camera3D{};
    std::reference_wrapper<Cool::AudioManager const> audio_manager;

    auto height() const -> float { return static_cast<float>(render_target_size.height()); }
    auto aspect_ratio() const -> float { return img::SizeU::aspect_ratio(render_target_size); }
    auto inverse_aspect_ratio() const -> float { return img::SizeU::aspect_ratio(render_target_size); }
};

} // namespace Lab
