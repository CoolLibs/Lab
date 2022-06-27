#pragma once

namespace Lab {

class DebugOptions {
public:
#if DEBUG
#include "generated/getters_for_debug_build.inl"
#else
#include "generated/getters_for_release_build.inl"
#endif

private:
#if DEBUG
    struct Instance {
#include "generated/debug_options_variables.inl"
    };
    static auto instance() -> Instance&
    {
        static Instance inst; // TODO serialize and read from file
        return inst;
    }
#endif

    friend class DebugOptionsDetails; // We go through this indirection so that only the files which include "DebugOptionsDetails" can call `imgui_checkboxes_for_all_options()`
    static void imgui_checkboxes_for_all_options()
    {
#if DEBUG
        // TODO Add a search bar
#include "generated/imgui_checkboxes_for_all_options.inl"
#endif
    }
};

} // namespace Lab