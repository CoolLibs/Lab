#include "Camera2DManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include "Cool/Dependencies/Input.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Input/MouseDragStartEvent.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "imgui.h"

namespace Lab {

void hook_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    Cool::Camera2D&                                    camera,
    std::function<void()>                              on_change,
    float&                                             sensitivity
)
{
    events
        .scroll_event()
        .subscribe([&, on_change](Cool::MouseScrollEvent<Cool::ViewCoordinates> const& event) {
            Cool::Log::Debug::info("fdg", "fkh");
            if (event.dy > 0)
                camera.zoom *= sensitivity;
            else
                camera.zoom /= sensitivity;
            on_change();
        });
    events
        .drag()
        .update()
        .subscribe([&, on_change](Cool::MouseDragUpdateEvent<Cool::ViewCoordinates> const& event) {
            if (event.delta != glm::vec2(0))
            {
                camera.translation += (event.delta) * glm::vec2(-0.001, -0.001);
            }
            on_change();
        });
}

} // namespace Lab