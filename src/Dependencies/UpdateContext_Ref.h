#pragma once
#include <Cool/Dependencies/Dirty.h>
#include <Cool/MessageConsole/MessageConsole.h>

namespace Lab {

class UpdateContext_Ref {
public:
    auto message_console() -> Cool::MessageConsole& { return _data.message_console; }
    void set_clean(const Cool::DirtyFlag& dirty_flag) { _data.set_clean(dirty_flag); }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<Cool::MessageConsole> message_console;
        Cool::SetClean_Ref                           set_clean;
    };
    explicit UpdateContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab