#pragma once
#include <Cool/Nodes/GetNodeDefinition_Ref.h>
#include <Cool/Nodes/NodesGraph.h>
#include "Function.h"
#include "NodeDefinition.h"

namespace Lab {

class CodeGenContext {
public:
    CodeGenContext(
        Cool::NodesGraph const&                     graph,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition
    )
        : _graph{graph}
        , _get_node_definition{get_node_definition}
    {}

    auto graph() const -> Cool::NodesGraph const& { return _graph; }
    auto get_node_definition(Cool::NodeDefinitionIdentifier const& id_names) const -> NodeDefinition const* { return _get_node_definition(id_names); }

    /// Adds the code of the function to the global code, and returns its name as a convenience.
    auto push_function(Function const&) -> std::string; // Allows us to add checks like making sure the same function isn't generated twice

    auto code() const -> std::string const& { return _code; }
    auto code() -> std::string& { return _code; }

private:
    class AlreadyGeneratedFunctions {
    public:
        void push(std::string const& name);
        auto has_already_been_generated(std::string const& name) const -> bool;

    private:
        std::vector<std::string> _names;
    };

private:
    std::string               _code{};
    AlreadyGeneratedFunctions _already_generated_functions{};

    Cool::NodesGraph const&                     _graph;
    Cool::GetNodeDefinition_Ref<NodeDefinition> _get_node_definition;
};

} // namespace Lab