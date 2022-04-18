#pragma once

/// This file isn't called "Time.h" because it would conflict with the C standard library's "time.h"

namespace Lab {

struct Time {
    float local;
    float global;
};

} // namespace Lab