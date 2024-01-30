#pragma once
#include <Cool/Dependencies/InputProvider_Ref.h>
#include "Cool/Nodes/NodesLibrary.h"
#include "Ui.h"

namespace Lab {

class UpdateContext_Ref {
public:
    auto hacky_input_provider() const { return _data.input_provider; } // TODO(Variables) Can we remove this? // HACK some values like time, aspect ratio and camera are not set properly in this input provider!!! Should only be used for the code generation, where we need to inject the values of some inputs (like MathExpression) at code-generation time.
    auto ui() const { return _data.ui; }
    auto nodes_library() const -> auto& { return _data.nodes_library.get(); }

    struct Data { // We wrap our members in a struct to get a constructor automatically
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