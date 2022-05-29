#include "CameraManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include "Dependencies/Commands.h"

namespace Lab {

void CameraManager::apply(float                              aspect_ratio,
                          std::reference_wrapper<Registries> registries,
                          CommandDispatcher                  commander)
{
    auto camera        = *registries.get().get(_camera_id);
    _last_aspect_ratio = aspect_ratio;
    _projection_controller.apply_to(camera, aspect_ratio);
    commander.dispatch(Command_SetValue{_camera_id, camera});
}

void CameraManager::hook_events(Cool::MouveEventDispatcher<Cool::ViewCoordinates>& events,
                                std::reference_wrapper<Registries>                 registries,
                                CommandDispatcher                                  commander)
{
    events
        .scroll_event()
        .subscribe([registries, camera_id = _camera_id, this, commander](const auto& event) {
            maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
                return _view_controller.on_wheel_scroll(camera, event.dy);
            });
            commander.dispatch(Command_FinishedEditingValue{});
        });
    events
        .drag()
        .start()
        .subscribe([registries, camera_id = _camera_id, this, commander](const auto& event) {
            maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
                return _view_controller.on_drag_start(camera, event.mods);
            });
        });
    events
        .drag()
        .update()
        .subscribe([registries, camera_id = _camera_id, this, commander](const auto& event) {
            maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
                return _view_controller.on_drag(camera, event.delta);
            });
        });
    events
        .drag()
        .stop()
        .subscribe([registries, this, commander](const auto& event) {
            maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
                return _view_controller.on_drag_stop(camera);
            });
            commander.dispatch(Command_FinishedEditingValue{});
        });
}

void CameraManager::imgui(std::reference_wrapper<Registries> registries,
                          CommandDispatcher                  commander)
{
    maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
        return _view_controller.ImGui(camera);
    });
    if (ImGui::Button("Look at the origin")) {
        maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
            _view_controller.set_orbit_center({0, 0, 0}, camera);
            return true;
        });
    }
    if (ImGui::Button("Reset transform")) {
        maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
            Cool::ViewController_OrbitalU::reset_transform(_view_controller, camera);
            _projection_controller.apply_to(camera, _last_aspect_ratio);
            return true;
        });
    }
    if (_projection_controller.ImGui()) {
        maybe_update_camera(registries, commander, [&](Cool::Camera& camera) {
            _projection_controller.apply_to(camera, _last_aspect_ratio);
            return true;
        });
    }
}

void CameraManager::maybe_update_camera(
    std::reference_wrapper<Registries> registries,
    CommandDispatcher                  commander,
    std::function<bool(Cool::Camera&)> fun)
{
    auto camera = *registries.get().get(_camera_id);
    if (fun(camera)) {
        commander.dispatch(Command_SetValue{_camera_id, camera});
    }
}

} // namespace Lab
