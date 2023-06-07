#pragma once
#include <string>
#include "FunctionSignature.h"

namespace Lab {

struct FunctionInput_DefinitionData {
    std::string       name{};
    FunctionSignature signature{};
    std::string       description{}; // TODO(JF) Display description on the node pin
    bool              is_main_input{};
};

class FunctionInput_Definition {
public:
    FunctionInput_Definition(FunctionInput_DefinitionData const&);

    auto name() const -> auto const& { return _data.name; }
    auto signature() const -> auto const& { return _data.signature; }
    auto is_main_input() const -> bool { return _data.is_main_input; }
    void set_description(std::string const& description) { _data.description = description; }

private:
    FunctionInput_DefinitionData _data;
};

} // namespace Lab
