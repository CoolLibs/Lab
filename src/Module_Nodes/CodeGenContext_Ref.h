#pragma once
#include <Cool/Dependencies/InputProvider_Ref.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include "Function.h"
#include "Graph.h"
#include "NodeDefinition.h"

namespace Lab {

struct CodeGenContext_Ref {
    Graph const&                                graph;
    Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition;
    Cool::InputProvider_Ref                     input_provider;
    AlreadyGeneratedFunctions&                  already_generated_functions;
};

class InputFunctionGenerator_Ref {
public:
    InputFunctionGenerator_Ref(CodeGenContext_Ref context, Node const& node)
        : _context{context}
        , _node{node}
    {}

    auto operator()(FunctionSignature) -> tl::expected<Function, std::string>;

private:
    CodeGenContext_Ref                 _context;
    std::reference_wrapper<Node const> _node;
};

class DefaultFunctionGenerator_Ref {
public:
    explicit DefaultFunctionGenerator_Ref(CodeGenContext_Ref context)
        : _context{context}
    {}

    auto operator()(FunctionSignature) -> tl::expected<Function, std::string>;

private:
    CodeGenContext_Ref _context;
};

} // namespace Lab