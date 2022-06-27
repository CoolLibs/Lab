#pragma once

#include "DebugOptions.h"

namespace Lab {

class DebugOptionsDetails {
public:
    static void imgui_checkboxes_for_all_options()
    {
        DebugOptions::imgui_checkboxes_for_all_options();
    }
};

} // namespace Lab
