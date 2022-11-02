#pragma once
#include <Cool/Dependencies/Dirty.h>

namespace Lab {

class UpdateContext_Ref {
public:
    void set_clean(Cool::DirtyFlag const& dirty_flag) { _data.set_clean(dirty_flag); }
    void set_dirty(Cool::DirtyFlag const& dirty_flag) { _data.set_dirty(dirty_flag); }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<Cool::MessageConsole> message_console;
        Cool::SetClean_Ref                           set_clean;
        Cool::SetDirty_Ref                           set_dirty;
    };
    explicit UpdateContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab