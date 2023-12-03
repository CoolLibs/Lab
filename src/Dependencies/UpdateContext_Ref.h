#pragma once
#include <Cool/Dependencies/Dirty.h>
#include <Cool/Dependencies/InputProvider_Ref.h>
#include "Cool/Nodes/NodesLibrary.h"
#include "Ui.h"

namespace Lab {

class UpdateContext_Ref {
public:
    void set_clean(Cool::DirtyFlag const& dirty_flag) { _data.set_clean(dirty_flag); }
    void set_dirty(Cool::DirtyFlag const& dirty_flag) { _data.set_dirty(dirty_flag); }
    auto dirty_setter() const { return _data.set_dirty; }
    auto input_provider() const { return _data.input_provider; }
    auto ui() const { return _data.ui; }
    auto nodes_library() const -> auto& { return _data.nodes_library.get(); }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        Cool::SetClean_Ref                         set_clean;
        Cool::SetDirty_Ref                         set_dirty;
        Cool::InputProvider_Ref                    input_provider;
        Ui_Ref                                     ui;
        std::reference_wrapper<Cool::NodesLibrary> nodes_library;
    };
    explicit UpdateContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab