#pragma once
#include <Cool/Dependencies/InputProvider_Ref.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include "Function.h"
#include "Graph.h"
#include "NodeDefinition.h"

namespace Lab {

namespace internal {
class AlreadyGeneratedFunctions {
public:
    AlreadyGeneratedFunctions(){};

    void push(std::string const& name);
    auto has_already_been_generated(std::string const& name) const -> bool;

private:
    std::vector<std::string> _names;
};
} // namespace internal

class CodeGenContext {
public:
    CodeGenContext(
        Graph const&                                graph,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        Cool::InputProvider_Ref                     input_provider
    )
        : _graph{graph}
        , _get_node_definition{get_node_definition}
        , _input_provider{input_provider}
    {}

    auto graph() const -> Graph const& { return _graph; }
    auto get_node_definition(std::string_view definition_name) const -> NodeDefinition const* { return _get_node_definition(definition_name); }
    auto input_provider() const -> Cool::InputProvider_Ref { return _input_provider; }

    /// Adds the code of the function to the global code, and returns its name as a convenience.
    auto push_function(Function const&) -> std::string; // Allows us to add checks like making sure the same function isn't generated twice

    auto code() const -> std::string { return _code; }

private:
    std::string                         _code{};
    internal::AlreadyGeneratedFunctions _already_generated_functions{};

    Graph const&                                _graph;
    Cool::GetNodeDefinition_Ref<NodeDefinition> _get_node_definition;
    Cool::InputProvider_Ref                     _input_provider;
};

class InputFunctionGenerator_Ref {
public:
    InputFunctionGenerator_Ref(CodeGenContext& context, Node const& node)
        : _context{context}
        , _node{node}
    {}

    auto operator()(FunctionSignature) -> ExpectedFunctionName;

private:
    std::reference_wrapper<CodeGenContext> _context;
    std::reference_wrapper<Node const>     _node;
};

class DefaultFunctionGenerator_Ref {
public:
    explicit DefaultFunctionGenerator_Ref(CodeGenContext& context)
        : _context{context}
    {}

    auto operator()(FunctionSignature) -> ExpectedFunctionName;

private:
    std::reference_wrapper<CodeGenContext> _context;
};

} // namespace Lab