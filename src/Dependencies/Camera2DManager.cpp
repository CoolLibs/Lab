#include "Camera2DManager.h"
#include <Cool/Camera/ViewController_OrbitalU.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Cool/Dependencies/Input.h"
#include "Cool/Image/AspectRatio.h"
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Input/MouseDragStartEvent.h"
#include "Cool/StrongTypes/Camera2D.h"
#include "glm/gtx/string_cast.hpp"
#include "imgui.h"

namespace Lab {

void hook_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    Cool::Camera2D&                                    camera,
    std::function<void()>                              on_change,
    std::function<float()>                             get_height,
    std::function<float()>                             get_aspect_ratio,
    float&                                             sensitivity
)

{
    events
        // TO DO (LC) shift+scroll and explanation
        .scroll_event()
        .subscribe([&, on_change, get_height, get_aspect_ratio](Cool::MouseScrollEvent<Cool::ViewCoordinates> const& event) {
            float tmp_zoom;
            if (event.dy > 0)
                tmp_zoom = sensitivity;
            else
                tmp_zoom = 1.f / sensitivity;

            auto const mouse_pos_in_view_space  = glm::vec2{event.position / get_height() * 2.f} - glm::vec2(get_aspect_ratio(), 1.f);
            glm::mat3  view_matrix              = camera.transform_matrix();
            auto const mouse_pos_in_world_space = glm::vec2{view_matrix * glm::vec3{mouse_pos_in_view_space, 1.f}};
            Cool::Log::Debug::info("World", glm::to_string(mouse_pos_in_world_space));

            float new_zoom = camera.zoom * tmp_zoom;

            camera.translation = camera.translation * (1.f / tmp_zoom) + mouse_pos_in_world_space - (1.f / tmp_zoom) * mouse_pos_in_world_space;
            camera.zoom        = new_zoom;

            // glm::mat3 const scale                   = glm::scale(glm::mat3{1.f}, glm::vec2{tmp_zoom});
            // auto const      mouse_pos_in_view_space = glm::vec2{event.position / get_height() * 2.f} - glm::vec2(get_aspect_ratio(), 1.f);
            // Cool::Log::Debug::info("View", glm::to_string(mouse_pos_in_view_space));
            // auto const mouse_pos_in_world_space = glm::vec2{camera.transform_matrix() * glm::vec3{mouse_pos_in_view_space, 1.f}};
            // Cool::Log::Debug::info("World", glm::to_string(mouse_pos_in_world_space));
            // glm::mat3 const translate = glm::translate(glm::mat3{1.f}, mouse_pos_in_world_space - camera.translation);
            // glm::mat3 const inverse   = glm::inverse(translate);
            // // auto const      new_transform            = camera.transform_matrix() * translate * scale;
            // auto const new_transform = camera.transform_matrix() * translate * scale * inverse;

            // camera.translation = glm::vec2(new_transform[2][0], new_transform[2][1]) / new_transform[2][2];
            on_change();
        });
    events
        .drag()
        .update()
        .subscribe([&, on_change, get_height](Cool::MouseDragUpdateEvent<Cool::ViewCoordinates> const& event) {
            if (event.delta != glm::vec2(0))
            {
                camera.translation -= ((event.delta) / get_height() * 2.f) / camera.zoom;
            }
            on_change();
        });
}

} // namespace Lab