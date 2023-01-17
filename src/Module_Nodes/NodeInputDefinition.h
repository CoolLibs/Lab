#pragma once
#include <string>
#include "FunctionSignature.h"

namespace Lab {

struct NodeInputDefinition_Data {
    std::string       name{};
    FunctionSignature signature{};
    std::string       description{"cacaboudin"};
    // TODO desctipyion
};

class NodeInputDefinition {
public:
    NodeInputDefinition(NodeInputDefinition_Data const&);

    auto name() const -> auto const& { return _data.name; }
    auto signature() const -> auto const& { return _data.signature; }
    void set_description(const std::string& description) { _data.description = description; }

private:
    NodeInputDefinition_Data _data;
};

} // namespace Lab
