#pragma once

#include "InputSlot.h"

namespace Lab {

class SetVariableDirty {
public:
    SetVariableDirty(AllInputSlots slots, SetDirtyFlag_Ref set_dirty)
        : _slots{slots}
        , _set_dirty{set_dirty}
    {
    }

    template<typename T>
    void operator()(reg::Id<T> variable_id)
    {
        for (const auto& slot : _slots) {
            const auto* slot_of_right_type = std::get_if<std::reference_wrapper<const Lab::InputSlot<T>>>(&slot);
            if (slot_of_right_type) {
                slot_of_right_type->get().set_dirty_if_depends_on(variable_id, _set_dirty);
            }
        }
    }

private:
    AllInputSlots    _slots;
    SetDirtyFlag_Ref _set_dirty;
};

} // namespace Lab
