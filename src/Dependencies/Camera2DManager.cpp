#include "Camera2DManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Cool/Dependencies/Input.h"
#include "Cool/Image/AspectRatio.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Input/MouseDragStartEvent.h"
#include "Cool/StrongTypes/Angle.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "Cool/UserSettings/UserSettings.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/string_cast.hpp"
#include "imgui.h"

namespace Lab {

void hook_camera2D_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    Cool::Camera2D&                                    camera,
    std::function<void()>                              on_change,
    std::function<float()>                             get_height,
    std::function<float()>                             get_aspect_ratio,
    std::function<bool()>                              is_locked_in_view
)
{
    events
        .scroll_event()
        .subscribe([&, on_change, get_height, get_aspect_ratio, is_locked_in_view](Cool::MouseScrollEvent<Cool::ViewCoordinates> const& event) {
            if (is_locked_in_view())
                return;

            if (ImGui::GetIO().KeyShift)
            {
                float const sensitivity        = Cool::user_settings().camera2D_zoom_sensitivity;
                float const rotation_variation = (sensitivity - 1.f) * event.dy;

                auto const mouse_pos_in_view_space  = event.position / get_height() * 2.f - glm::vec2{get_aspect_ratio(), 1.f};
                auto const mouse_pos_in_world_space = glm::vec2{/* glm::inverse */(camera.transform_matrix()) * glm::vec3{mouse_pos_in_view_space, 1.f}};
                auto const rotated_mouse_in_ws      = glm::rotate(mouse_pos_in_world_space, -camera.rotation.as_radians());

                // camera.translation = camera.translation / zoom_variation + rotated_mouse_in_ws * (1.f - 1.f / zoom_variation);
                // camera.translation = glm::rotate(camera.translation, -rotation_variation)
                //                      - glm::rotate(rotated_mouse_in_ws, -rotation_variation)
                //                      + rotated_mouse_in_ws;
                camera.translation = mouse_pos_in_world_space + glm::rotate(camera.translation - mouse_pos_in_world_space, -rotation_variation);
                camera.rotation    = Cool::Radians{camera.rotation.as_radians() + rotation_variation};
            }
            else
            {
                float const sensitivity    = Cool::user_settings().camera2D_zoom_sensitivity;
                float const zoom_variation = std::pow(sensitivity, event.dy);

                auto const mouse_pos_in_view_space  = event.position / get_height() * 2.f - glm::vec2{get_aspect_ratio(), 1.f};
                auto const mouse_pos_in_world_space = glm::vec2{camera.transform_matrix() * glm::vec3{mouse_pos_in_view_space, 1.f}};
                auto const rotated_mouse_in_ws      = glm::rotate(mouse_pos_in_world_space, -camera.rotation.as_radians());

                camera.translation = rotated_mouse_in_ws + (camera.translation - rotated_mouse_in_ws) / zoom_variation;
                camera.zoom *= zoom_variation;
            }

            on_change();
        });

    events
        .drag()
        .update()
        .subscribe([&, on_change, get_height, is_locked_in_view](Cool::MouseDragUpdateEvent<Cool::ViewCoordinates> const& event) {
            if (is_locked_in_view())
                return;

            camera.translation -= event.delta / get_height() * 2.f / camera.zoom;
            on_change();
        });

    // TODO rotation when shift+scroll and explanation
}

} // namespace Lab