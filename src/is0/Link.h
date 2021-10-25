#pragma once

#include "Ed.h"

struct Link {
    ed::LinkId id;
    ed::PinId  from_pin_id;
    ed::PinId  to_pin_id;
};
