#pragma once

#include "FunctionSignature.h"
#include "GlslCode.h"
#include "NodeInput.h"

namespace Lab {

struct NodeDefinition {
    const std::string       name;
    const FunctionSignature signature;
    const GlslCode          function_body;

    const std::vector<NodeInput> inputs;
};

} // namespace Lab
