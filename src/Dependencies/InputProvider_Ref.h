#pragma once

#include "Input.h"
#include "VariableRegistries.h"

namespace Lab {

class InputProvider_Ref {
public:
    InputProvider_Ref(const VariableRegistries& registries, float render_target_aspect_ratio, float time)
        : _variable_registries{registries}
        , _render_target_aspect_ratio{render_target_aspect_ratio}
        , _time{time}
    {
    }

    template<typename T>
    auto operator()(const Input<T>& input) const -> T
    {
        { // Try the current variable
            const auto maybe_variable = _variable_registries.get().get(input._current_variable_id);
            if (maybe_variable)
            {
                return maybe_variable->value;
            }
        }
        { // Try the default variable
            const auto maybe_variable = _variable_registries.get().get(input._default_variable_id);
            if (maybe_variable)
            {
                return maybe_variable->value;
            }
        }
        // Default, this should not happen
        Cool::Log::error("InputProvider_Ref", "An Input has an invalid default variable!");
        return T{};
    }

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
};

} // namespace Lab
