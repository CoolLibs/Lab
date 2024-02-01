#pragma once
#include <functional> // For std::reference_wrapper
#include "Cool/Audio/AudioManager.h"
#include "Cool/Camera/Camera.h"

namespace Lab {

struct SystemValues {
    float                                            render_target_aspect_ratio{}; // TODO(Variables) Remove, this is redundant with render_target_size
    img::Size                                        render_target_size{};
    float                                            height{}; // TODO(Variables) Remove, this is redundant with render_target_size
    float                                            time{};
    float                                            delta_time{};
    glm::mat3                                        camera2D{};
    Cool::Camera                                     camera3D{};
    std::reference_wrapper<Cool::AudioManager const> audio_manager;
};

} // namespace Lab
