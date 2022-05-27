#pragma once

#include <Cool/Camera/CameraManager.h>
#include <reg/reg.hpp>
#include "Dependencies/CommandsExec.h"

namespace Lab {

class CameraManager {
public:
    void update(CommandDispatcher) const;

    void set_camera(const Cool::Camera&);

private:
    Cool::CameraManager   _camera_manager;
    Cool::Camera          _previous_camera;
    reg::Id<Cool::Camera> _id;
};

} // namespace Lab