#pragma once

#include "DebugOptions.h"

namespace Lab {

class DebugOptionsDetails {
public:
    static void imgui_checkboxes_for_all_options()
    {
        DebugOptions::imgui_checkboxes_for_all_options();
    }

    struct AutoSerializer {
        ~AutoSerializer()
        {
            Cool::Serialization::to_json(
                DebugOptions::instance(),
                Cool::Path::root() + "/cache--debug-options.json",
                "Debug Options"
            );
        }
    };

    static void reset_all()
    {
        DebugOptions::reset_all();
    }
};

} // namespace Lab
