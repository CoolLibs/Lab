#include "CameraManager.h"
#include "Dependencies/Commands.h"

namespace Lab {

void CameraManager::apply(float                              aspect_ratio,
                          std::reference_wrapper<Registries> registries,
                          CommandDispatcher                  commander)
{
    auto camera = *registries.get().get(_camera_id);
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
            auto camera = *registries.get().get(camera_id);
            if (_view_controller.on_wheel_scroll(camera, event.dy)) {
                commander.dispatch(Command_SetValue{camera_id, camera});
                commander.dispatch(Command_FinishedEditingValue{});
            }
        });
    events
        .drag()
        .start()
        .subscribe([registries, camera_id = _camera_id, this, commander](const auto& event) {
            auto camera = *registries.get().get(camera_id);
            if (_view_controller.on_drag_start(camera, event.mods)) {
                commander.dispatch(Command_SetValue{camera_id, camera});
            }
        });
    events
        .drag()
        .update()
        .subscribe([registries, camera_id = _camera_id, this, commander](const auto& event) {
            auto camera = *registries.get().get(camera_id);
            if (_view_controller.on_drag(camera, event.delta)) {
                commander.dispatch(Command_SetValue{camera_id, camera});
            }
        });
    events
        .drag()
        .stop()
        .subscribe([registries, camera_id = _camera_id, this, commander](const auto& event) {
            auto camera = *registries.get().get(camera_id);
            if (_view_controller.on_drag_stop(camera)) {
                commander.dispatch(Command_SetValue{camera_id, camera});
            }
            commander.dispatch(Command_FinishedEditingValue{});
        });
}

void CameraManager::imgui()
{
    // _view_controller.ImGui(_camera);
    // if (ImGui::Button("Look at the origin")) {
    //     _view_controller.set_orbit_center({0, 0, 0}, _camera);
    // }
    // if (ImGui::Button("Reset transform")) {
    //     reset_transform();
    // }
    // _projection_controller.ImGui();
}

} // namespace Lab
