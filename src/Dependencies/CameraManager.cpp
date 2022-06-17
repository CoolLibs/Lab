#include "CameraManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include "CommandCore/CommandExecutor_TopLevel_Ref.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetVariable.h"

namespace Lab {

void CameraManager::hook_events(Cool::MouveEventDispatcher<Cool::ViewCoordinates>& events,
                                std::reference_wrapper<VariableRegistries>         registries,
                                CommandExecutor_TopLevel_Ref                       executor)
{
    events
        .scroll_event()
        .subscribe([registries, this, executor](const auto& event) {
            maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
                return _view_controller.on_wheel_scroll(camera, event.dy);
            });
            executor.execute(Command_FinishedEditingVariable{});
        });
    events
        .drag()
        .start()
        .subscribe([registries, this, executor](const auto& event) {
            maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
                return _view_controller.on_drag_start(camera, event.mods);
            });
        });
    events
        .drag()
        .update()
        .subscribe([registries, this, executor](const auto& event) {
            maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
                return _view_controller.on_drag(camera, event.delta);
            });
        });
    events
        .drag()
        .stop()
        .subscribe([registries, this, executor](auto&&) {
            maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
                return _view_controller.on_drag_stop(camera);
            });
            executor.execute(Command_FinishedEditingVariable{});
        });
}

void CameraManager::imgui(std::reference_wrapper<VariableRegistries> registries,
                          CommandExecutor_TopLevel_Ref               executor)
{
    maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
        return _view_controller.ImGui(camera);
    });
    if (ImGui::Button("Look at the origin")) {
        maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
            _view_controller.set_orbit_center({0, 0, 0}, camera);
            return true;
        });
        executor.execute(Command_FinishedEditingVariable{});
    }
    if (ImGui::Button("Reset transform")) {
        maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
            Cool::ViewController_OrbitalU::reset_transform(_view_controller, camera);
            return true;
        });
        executor.execute(Command_FinishedEditingVariable{});
    }
    maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
        return Cool::imgui(camera.projection());
    });
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        executor.execute(Command_FinishedEditingVariable{});
    }
}

void CameraManager::maybe_update_camera(
    std::reference_wrapper<VariableRegistries> registries,
    CommandExecutor_TopLevel_Ref               executor,
    std::function<bool(Cool::Camera&)>         fun)
{
    auto camera = registries.get().get(_camera_id)->value;
    if (fun(camera)) {
        executor.execute(Command_SetVariable<Cool::Camera>{_camera_id, camera});
    }
}

} // namespace Lab
