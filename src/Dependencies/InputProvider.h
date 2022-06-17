#pragma once

#include "Input.h"
#include "VariableRegistries.h"

namespace Lab {

class InputProvider {
public:
    InputProvider(const VariableRegistries& registries, float render_target_aspect_ratio, float time, const reg::Id<Cool::Variable<Cool::Camera>>& camera_id)
        : _variable_registries{registries}
        , _render_target_aspect_ratio{render_target_aspect_ratio}
        , _time{time}
        , _camera_id{camera_id}
    {
    }

    template<typename T>
    auto operator()(const Input<T>& input) const -> T
    {
        const auto maybe_variable = _variable_registries.get().get(input._variable_id);
        if (maybe_variable) {
            return maybe_variable->value;
        }
        else {
            return T{};
        }
    }

    template<>
    auto operator()(const Input<Cool::Camera>&) const -> Cool::Camera;

    float operator()(const Input_AspectRatio&) const
    {
        return _render_target_aspect_ratio;
    }

    auto operator()(const Input_Time&) const -> float
    {
        return _time;
    }

    auto operator()(const Input_File& file_input) const -> std::filesystem::path
    {
        return file_input.file_watcher.path();
    }

private:
    std::reference_wrapper<const VariableRegistries> _variable_registries;
    float                                            _render_target_aspect_ratio;
    float                                            _time;
    reg::Id<Cool::Variable<Cool::Camera>>            _camera_id;
};

} // namespace Lab
