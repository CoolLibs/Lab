#pragma once
#include <Cool/Dependencies/InputProvider_Ref.h>
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/Graph.h>
#include "Function.h"
#include "NodeDefinition.h"

namespace Lab {

class CodeGenContext {
public:
    CodeGenContext(
        Cool::Graph const&                          graph,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        Cool::InputProvider_Ref                     input_provider
    )
        : _graph{graph}
        , _get_node_definition{get_node_definition}
        , _input_provider{input_provider}
    {}

    auto graph() const -> Cool::Graph const& { return _graph; }
    auto get_node_definition(Cool::NodeDefinitionIdentifier const& id_names) const -> NodeDefinition const* { return _get_node_definition(id_names); }
    auto input_provider() const -> Cool::InputProvider_Ref { return _input_provider; }

    /// Adds the code of the function to the global code, and returns its name as a convenience.
    auto push_function(Function const&) -> std::string; // Allows us to add checks like making sure the same function isn't generated twice

    auto code() const -> std::string { return _code; }

    auto function_names() const -> std::vector<std::string> const& { return _already_generated_functions.names(); }

private:
    class AlreadyGeneratedFunctions {
    public:
        AlreadyGeneratedFunctions(){};

        void push(std::string const& name);
        auto has_already_been_generated(std::string const& name) const -> bool;

        auto names() const -> std::vector<std::string> const& { return _names; }

    private:
        std::vector<std::string> _names;
    };

private:
    std::string               _code{};
    AlreadyGeneratedFunctions _already_generated_functions{};

    Cool::Graph const&                          _graph;
    Cool::GetNodeDefinition_Ref<NodeDefinition> _get_node_definition;
    Cool::InputProvider_Ref                     _input_provider;
};

} // namespace Lab