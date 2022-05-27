#include "CameraManager.h"
#include "Dependencies/Commands.h"

namespace Lab {

void CameraManager::update(CommandDispatcher dispatcher) const
{
    if (_previous_camera != *_camera_manager) {
        dispatcher.dispatch(Command_SetValue{_id, *_camera_manager});
    }
}

void CameraManager::set_camera(const Cool::Camera& camera)
{
    _previous_camera = *_camera_manager;
    *_camera_manager = camera;
}

} // namespace Lab
