#pragma once
#include "Cool/Nodes/NodesLibrary.h"
#include "Ui.h"

namespace Lab {

class UpdateContext_Ref {
public:
    auto ui() const { return _data.ui; }
    auto nodes_library() const -> auto& { return _data.nodes_library.get(); }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        Ui_Ref                                     ui;
        std::reference_wrapper<Cool::NodesLibrary> nodes_library;
    };
    explicit UpdateContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Lab