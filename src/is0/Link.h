#pragma once

#include <Cool/Uuid/Uuid.h>
#include "Ed.h"

struct Link {
    ed::LinkId id = static_cast<ed::LinkId>(Cool::Uuid{});
    ed::PinId  from_pin_id;
    ed::PinId  to_pin_id;
};
