#include "CameraManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/CommandExecutor_TopLevel_Ref.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetCameraZoom.h"
#include "Commands/Command_SetVariable.h"

namespace Lab {

void CameraManager::hook_events(
    Cool::MouveEventDispatcher<Cool::ViewCoordinates>& events,
    std::reference_wrapper<Cool::VariableRegistries>   registries,
    CommandExecutor_TopLevel_Ref                       executor
)
{
    events
        .scroll_event()
        .subscribe([registries, this, executor](const auto& event) {
            auto       camera   = registries.get().get(_camera_id)->value;
            const auto old_zoom = _view_controller.get_distance_to_orbit_center();
            if (_view_controller.on_wheel_scroll(camera, event.dy))
            {
                const auto zoom = _view_controller.get_distance_to_orbit_center();
                _view_controller.set_distance_to_orbit_center(old_zoom); // Undo the zoom, it will be done by the Command_SetCameraZoom
                executor.execute(Command_SetCameraZoom{zoom});
                executor.execute(Command_FinishedEditingVariable{});
            }
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

void CameraManager::imgui(
    std::reference_wrapper<Cool::VariableRegistries> registries,
    CommandExecutor_TopLevel_Ref                     executor
)
{
    maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
        return _view_controller.ImGui(camera);
    });
    if (ImGui::Button("Look at the origin"))
    {
        maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
            _view_controller.set_orbit_center({0, 0, 0}, camera);
            return true;
        });
        executor.execute(Command_FinishedEditingVariable{});
    }
    if (ImGui::Button("Reset transform"))
    {
        maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
            Cool::ViewController_OrbitalU::reset_transform(_view_controller, camera);
            return true;
        });
        executor.execute(Command_FinishedEditingVariable{});
    }
    maybe_update_camera(registries, executor, [&](Cool::Camera& camera) {
        return Cool::imgui(camera.projection());
    });
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        executor.execute(Command_FinishedEditingVariable{});
    }
}

void CameraManager::maybe_update_camera(
    std::reference_wrapper<Cool::VariableRegistries> registries,
    CommandExecutor_TopLevel_Ref                     executor,
    std::function<bool(Cool::Camera&)>               fun
)
{
    auto camera = registries.get().get(_camera_id)->value;
    if (fun(camera))
    {
        executor.execute(Command_SetVariable<Cool::Camera>{_camera_id, camera});
    }
}

void CameraManager::set_zoom(float zoom, CommandExecutionContext_Ref& ctx)
{
    auto camera = ctx.registries().get(_camera_id)->value;
    Cool::ViewController_OrbitalU::set_distance_to_orbit_center(_view_controller, camera, zoom);
    ctx.registries().with_mutable_ref<Cool::Variable<Cool::Camera>>(_camera_id, [&](Cool::Variable<Cool::Camera>& variable) {
        variable.value = camera;
    });
    ctx.set_dirty(_camera_id);
}

} // namespace Lab
