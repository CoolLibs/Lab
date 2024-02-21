#pragma once
#include <Cool/Nodes/NodeId.h>
#include <Nodes/Node.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {

struct ReversibleCommand_ChangeNodeDefinition;

struct Command_ChangeNodeDefinition {
    Cool::NodeId node_id;
    Node         new_value;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_ChangeNodeDefinition;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Node id", node_id),
            cereal::make_nvp("New value", new_value)
        );
    }
};

struct ReversibleCommand_ChangeNodeDefinition {
    Command_ChangeNodeDefinition fwd;
    Node                         old_value;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    void revert(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto merge(ReversibleCommand_ChangeNodeDefinition const&) const -> std::optional<ReversibleCommand_ChangeNodeDefinition>;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Forward", fwd),
            cereal::make_nvp("Old value", old_value)
        );
    }
};

} // namespace Lab