#pragma once
#include "Cool/Serialization/Json.hpp"
#include "Cool/Serialization/JsonAutoSerializer.hpp"

namespace Lab {

struct UserSettings {
    bool switch_to_new_project_when_saving_as{false};

    auto imgui() -> bool;
    auto imgui_switch_to_new_project_when_saving_as() -> bool;

    void update() { _serializer.update(); }
    void save() { _serializer.save(); }

private:
    // Must be declared last, after all the variables it serializes, so that the values it loads overwrite the default values, and not the other way around
    Cool::JsonAutoSerializer _serializer{
        "coollab_user_settings.json",
        false /*autosave_when_destroyed*/, // This is a static instance, so saving it in the destructor is dangerous because we don't know when it will happen exactly. Instead, we call save manually at the end of the run()
        [&](nlohmann::json const& json) {
            Cool::json_get(json, "Switch to the new project when saving as", switch_to_new_project_when_saving_as);
        },
        [&](nlohmann::json& json) {
            Cool::json_set(json, "Switch to the new project when saving as", switch_to_new_project_when_saving_as);
        },
        false /*use_shared_user_data*/
    };
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Lab