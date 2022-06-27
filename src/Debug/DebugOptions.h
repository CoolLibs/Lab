#pragma once

#include <Cool/Path/Path.h>
#include <Cool/Serialization/as_json.h>

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
        ImGuiTextFilter filter;

    private:
        // Serialization
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
#include "generated/cereal_make_nvp.inl"
            );
        }
    };

    static auto load_debug_options() -> Instance
    {
        auto the_instance = Instance{};
        Cool::Serialization::from_json(the_instance, Cool::Path::root() + "/cache--debug-options.json");
        return the_instance;
    }

    static auto instance() -> Instance&
    {
        static auto the_instance = Instance{load_debug_options()};
        return the_instance;
    }
#endif

    friend class DebugOptionsDetails; // We go through this indirection so that only the files which include "DebugOptionsDetails" can call `imgui_checkboxes_for_all_options()`
    static void imgui_checkboxes_for_all_options()
    {
#if DEBUG
        instance().filter.Draw("Filter");
#include "generated/imgui_checkboxes_for_all_options.inl"
#endif
    }
};

} // namespace Lab