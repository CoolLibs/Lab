#pragma once

#include "InputSlot.h"
#include "VariableRegistries.h"

namespace Lab {

class InputProvider {
public:
    InputProvider(const VariableRegistries& registries, float render_target_aspect_ratio, float time, const reg::Id<Cool::Camera>& camera_id)
        : _variable_registries{registries}
        , _render_target_aspect_ratio{render_target_aspect_ratio}
        , _time{time}
        , _camera_id{camera_id}
    {
    }

    template<typename T>
    auto operator()(const InputSlot<T>& slot) const -> T
    {
        const auto maybe_value = _variable_registries.get().get(slot.id);
        return maybe_value.value_or(T{});
    }

    template<>
    auto operator()(const InputSlot<Cool::Camera>& slot) const -> Cool::Camera;

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

private:
    std::reference_wrapper<const VariableRegistries> _variable_registries;
    float                                            _render_target_aspect_ratio;
    float                                            _time;
    reg::Id<Cool::Camera>                            _camera_id;
};

} // namespace Lab
