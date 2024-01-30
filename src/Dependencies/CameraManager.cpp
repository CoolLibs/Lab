#include "CameraManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <Cool/Dependencies/DirtyFlag.h>
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/ImGui/icon_fmt.h>
#include <memory>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/CommandExecutor.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetCameraZoom.h"
#include "Commands/Command_SetVariable.h"

namespace Lab {

void CameraManager::hook_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    CommandExecutor const&                             executor,
    std::function<void()> const&                       on_change // TODO(Variables) Shouldn't be needed if we set the DirtyFlag of the CameraInput properly
)
{
    events
        .scroll_event()
        .subscribe([this, executor, on_change](auto const& event) {
            if (!_is_editable_in_view)
                return;

            auto       camera   = _camera_input.value();
            auto const old_zoom = _view_controller.get_distance_to_orbit_center();
            if (_view_controller.on_wheel_scroll(camera, event.dy))
            {
                auto const zoom = _view_controller.get_distance_to_orbit_center();
                _view_controller.set_distance_to_orbit_center(old_zoom); // Undo the zoom, it will be done by the Command_SetCameraZoom
                executor.execute(Command_SetCameraZoom{zoom});
                executor.execute(Command_FinishedEditingVariable{});
                on_change();
            }
        });
    events
        .drag()
        .subscribe({
            .on_start  = [this, executor, on_change](auto const&) {
                if (!_is_editable_in_view)
                    return false;

                maybe_update_camera( executor, on_change, [&](Cool::Camera& camera) {
                    return _view_controller.on_drag_start(camera);
                });
                return true; },
            .on_update = [this, executor, on_change](auto const&) {
                //
                maybe_update_camera(executor, on_change, [&](Cool::Camera& camera) {
                    return _view_controller.on_drag(camera, ImGui::GetIO().MouseDelta); // NB: we don't use event.delta as it is in relative coordinates, and we want a delta in pixels to keep the drag speed the same no matter the size of the View.
                });
                ImGui::WrapMousePos(ImGuiAxesMask_All);
                //
            },
            .on_stop = [this, executor, on_change](auto&&) {
                maybe_update_camera( executor, on_change, [&](Cool::Camera& camera) {
                    return _view_controller.on_drag_stop(camera);
                });
                executor.execute(Command_FinishedEditingVariable{}); },
        });
}

void CameraManager::imgui(
    CommandExecutor const&       executor,
    std::function<void()> const& on_change
)
{
    maybe_update_camera(executor, on_change, [&](Cool::Camera& camera) {
        return _view_controller.ImGui(camera);
    });
    if (ImGui::Button(Cool::icon_fmt("Look at the origin", ICOMOON_RADIO_CHECKED).c_str()))
    {
        maybe_update_camera(executor, on_change, [&](Cool::Camera& camera) {
            _view_controller.set_orbit_center({0, 0, 0}, camera);
            return true;
        });
        executor.execute(Command_FinishedEditingVariable{});
    }
    if (ImGui::Button(Cool::icon_fmt("Reset Camera", ICOMOON_TARGET).c_str()))
    {
        reset_camera(executor, on_change);
    }
    maybe_update_camera(executor, on_change, [&](Cool::Camera& camera) {
        return Cool::imgui(camera.projection());
    });
    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        executor.execute(Command_FinishedEditingVariable{});
    }
}

void CameraManager::maybe_update_camera(
    CommandExecutor const&                    executor,
    std::function<void()> const&              on_change,
    std::function<bool(Cool::Camera&)> const& fun
)
{
    auto camera = _camera_input.value();
    if (fun(camera))
    {
        executor.execute(Command_SetVariable<Cool::Camera>{_camera_input, camera});
        on_change(); // TODO(Variables) Why do we need that if Camera has the right DirtyFlag that tells the App to rerender?
    }
}

void CameraManager::set_zoom(float zoom, CommandExecutionContext_Ref const&)
{
    Cool::ViewController_OrbitalU::set_distance_to_orbit_center(_view_controller, _camera_input.value(), zoom);
    _camera_input._dirty_flag.set_dirty();
}

void CameraManager::reset_camera(
    CommandExecutor const&       executor,
    std::function<void()> const& on_change
)
{
    maybe_update_camera(executor, on_change, [&](Cool::Camera& camera) {
        Cool::ViewController_OrbitalU::reset_transform(_view_controller, camera);
        return true;
    });
    executor.execute(Command_FinishedEditingVariable{});
}

} // namespace Lab
