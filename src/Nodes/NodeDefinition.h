#pragma once

#include "FunctionSignature.h"
#include "GlslCode.h"
#include "NodeInput.h"

namespace Lab {

struct NodeDefinition {
    const std::string       _name;
    const FunctionSignature signature;
    const GlslCode          function_body;

    const std::vector<NodeInput> inputs;

    auto name() const { return _name; }
};

} // namespace Lab
