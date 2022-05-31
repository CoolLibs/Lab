#include "InputProvider.h"

namespace Lab {

template<>
auto InputProvider::operator()(const InputSlot<Cool::Camera>& slot) const -> Cool::Camera
{
    slot.id = _camera_id;
    return _variable_registries.get().get(_camera_id).value_or(Cool::Camera{});
}

} // namespace Lab
