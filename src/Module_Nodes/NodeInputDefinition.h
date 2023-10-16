#pragma once
#include <string>
#include "FunctionSignature.h"

namespace Lab {

struct NodeVariableDefinition_Data {
    std::string       name{};
    FunctionSignature signature{};
    std::string       description{}; // TODO(JF) Display description on the node pin
};

class NodeVariableDefinition {
public:
    NodeVariableDefinition(NodeVariableDefinition_Data const&);

    auto name() const -> auto const& { return _data.name; }
    auto signature() const -> auto const& { return _data.signature; }
    void set_description(std::string const& description) { _data.description = description; }

private:
    NodeVariableDefinition_Data _data;
};

} // namespace Lab
