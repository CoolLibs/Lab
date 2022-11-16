#pragma once
#include "FunctionSignature.h"

namespace Lab {

struct NodeInputDefinition_Data {
    std::string       name{};
    FunctionSignature signature{};
};

class NodeInputDefinition {
public:
    NodeInputDefinition(NodeInputDefinition_Data const&);

    auto name() const -> auto const& { return _data.name; }
    auto signature() const -> auto const& { return _data.signature; }

private:
    NodeInputDefinition_Data _data;
};

} // namespace Lab
