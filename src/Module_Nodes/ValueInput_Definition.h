#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputDefinition.h>
#include <string>
#include "FunctionSignature.h"

namespace Lab {

struct ValueInput_DefinitionData {
    Cool::AnyInputDefinition input_def{};
    bool                     is_main_input{};
};

class ValueInput_Definition {
public:
    ValueInput_Definition(ValueInput_DefinitionData const&);

    auto input_def() const -> auto const& { return _data.input_def; }
    auto input_def() -> auto& { return _data.input_def; }
    auto is_main_input() const -> bool { return _data.is_main_input; }

private:
    ValueInput_DefinitionData _data;
};

} // namespace Lab
