#pragma once
#include <string>
#include "FunctionSignature.h"

namespace Lab {

struct NodeInputDefinition_Data {
    std::string       name{};
    FunctionSignature signature{};
    std::string       description{}; // TODO(JF) Display description on the node pin
};

class NodeInputDefinition {
public:
    NodeInputDefinition(NodeInputDefinition_Data const&);

    auto name() const -> auto const& { return _data.name; }
    auto description() const -> auto const& { return _data.description; }
    auto signature() const -> auto const& { return _data.signature; }
    void set_description(std::string const& description) { _data.description = description; }

private:
    NodeInputDefinition_Data _data;
};

} // namespace Lab
