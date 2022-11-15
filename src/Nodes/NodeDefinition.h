#pragma once

#include <Cool/Dependencies/Input.h>
#include <Cool/Dependencies/InputDefinition.h>
#include "FunctionSignature.h"
#include "NodeInput.h"

namespace Lab {

struct NodeDefinition_Data {
    const std::string       name{};
    const FunctionSignature signature{};
    const std::string       function_body{};

    const std::vector<NodeInput>                inputs{};     // Things that will get a pin
    const std::vector<Cool::AnyInputDefinition> properties{}; // Things that will just be sliders etc. on the node itself
};

class NodeDefinition {
public:
    NodeDefinition(NodeDefinition_Data const&);

    auto name() const -> std::string const& { return _data.name; }
    auto signature() const -> FunctionSignature const& { return _data.signature; }
    auto function_body() const -> std::string const& { return _data.function_body; }
    auto inputs() const -> std::vector<NodeInput> const& { return _data.inputs; }
    auto properties() const -> std::vector<Cool::AnyInputDefinition> const& { return _data.properties; }

private:
    NodeDefinition_Data _data;
};

} // namespace Lab
