#pragma once
#include <Cool/Nodes/NodeId.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"
#include "Nodes/Node.h"

namespace Lab {

struct ReversibleCommand_AddNode;

struct Command_AddNode {
    Cool::NodeId node_id;
    Node         node;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_AddNode;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Node id", node_id),
            ser20::make_nvp("Node", node)
        );
    }
};

struct ReversibleCommand_AddNode {
    Command_AddNode fwd;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    void revert(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto merge(ReversibleCommand_AddNode const&) const -> std::optional<ReversibleCommand_AddNode>;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Forward", fwd)
        );
    }
};

} // namespace Lab