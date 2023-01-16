#pragma once

#include "Cool/Input/MouseEventDispatcher.h"
#include "Cool/StrongTypes/Camera2D.h"

namespace Lab {

void hook_events(
    Cool::MouseEventDispatcher<Cool::ViewCoordinates>& events,
    Cool::Camera2D&                                    camera,
    std::function<void()>                              on_change,
    float&                                             sensitivity
);

}