#pragma once
#include <cereal/types/polymorphic.hpp>
#include <glm/glm.hpp>
#include <stringify/stringify.hpp>
// #include "Commands.h"
#include "Dependencies.h"
#include "Dependencies/History.h"
#include "Dependencies/InputSlot.h"
#include "Dependencies/Registries.h"
#include "Dependencies/Ui.h"

namespace Lab {

/// We need to do the polymorphism of Modules through inheritance to allow for plugins to create their own modules.

/// Each module has a State struct, and that's what is serialized / modified through commands / stored in presets.
/// The ui() method should be const, because it sould only trigger commands, not modify internal values (allows us to handle history / re-rendering at a higher level)
/// Rendering only has const access to the registries: creating / updating values is done trough ui()

struct InputSlot_AspectRatio {
};
struct InputSlot_Time {
};
struct InputSlot_Camera {
};

class InputProvider {
public:
    InputProvider(const Registries& registries, float render_target_aspect_ratio, float time, const reg::Id<Cool::Camera>& camera_id)
        : _registries{registries}
        , _render_target_aspect_ratio{render_target_aspect_ratio}
        , _time{time}
        , _camera_id{camera_id}
    {
    }

    template<typename T>
    T operator()(const InputSlot<T>& slot) const
    {
        const auto maybe_value = _registries.get().get(slot.id);
        return maybe_value.value_or(T{});
    }

    float operator()(const InputSlot_AspectRatio&) const
    {
        return _render_target_aspect_ratio;
    }

    auto operator()(const InputSlot_Time&) const -> float
    {
        return _time;
    }

    auto operator()(const InputSlot_File& file_input) const -> std::filesystem::path
    {
        return file_input.file_watcher.path();
    }

    auto operator()(const InputSlot_Camera&) const -> Cool::Camera
    {
        return _registries.get().get(_camera_id).value_or(Cool::Camera{});
    }

private:
    std::reference_wrapper<const Registries> _registries;
    float                                    _render_target_aspect_ratio;
    float                                    _time;
    reg::Id<Cool::Camera>                    _camera_id;
};

class Module {
public:
    struct RenderParams {
        InputProvider          provider;
        InputSlotDestructorRef input_slot_destructor;
        DirtyManager           dirty_manager;
    };

    virtual ~Module() = default;
    void do_rendering(RenderParams params)
    {
        render(params);
        _is_dirty = false;
    }
    virtual void imgui_windows(Ui ui) = 0;
    virtual void update(){};

    virtual auto is_dirty(DirtyManager) const -> bool { return _is_dirty; }

    virtual auto depends_on(reg::AnyId) const -> bool = 0;

    void set_dirty()
    {
        _is_dirty = true;
    }

protected:
    virtual void render(RenderParams) = 0;
    // template<typename T>
    // T get();

protected:
    // Dependencies _dependencies;
    bool _is_dirty = true;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // archive();
    }
};

template<typename T>
void imgui_show(const T& value)
{
    ImGui::Text("%s", Cool::stringify(value).c_str());
}

template<typename T>
inline void imgui_show(const reg::Id<T>& id)
{
    ImGui::Text("%s", reg::to_string(id).c_str());
}

template<typename T>
void imgui_show(const reg::Registry<T>& registry)
{
    std::shared_lock       lock{registry.mutex()};
    static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_ContextMenuInBody;

    if (ImGui::BeginTable("table1", 2, flags)) {
        for (const auto& [id, value] : registry) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            imgui_show(value);
            ImGui::TableSetColumnIndex(1);
            imgui_show(id);
        }
        ImGui::EndTable();
    }
}

} // namespace Lab