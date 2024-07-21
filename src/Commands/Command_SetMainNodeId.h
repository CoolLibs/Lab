#pragma once
#include <Cool/Nodes/NodeId.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {

struct ReversibleCommand_SetMainNodeId;

struct Command_SetMainNodeId {
    Cool::NodeId main_node_id;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_SetMainNodeId;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Main node id", main_node_id)
        );
    }
};

struct ReversibleCommand_SetMainNodeId {
    Command_SetMainNodeId fwd;
    Cool::NodeId          old_main_node_id;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    void revert(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto merge(ReversibleCommand_SetMainNodeId const&) const -> std::optional<ReversibleCommand_SetMainNodeId>;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Forward", fwd),
            ser20::make_nvp("Old main node id", old_main_node_id)
        );
    }
};

} // namespace Lab