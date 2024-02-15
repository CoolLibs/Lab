#pragma once
#include <Cool/Nodes/Link.h>
#include <Cool/Nodes/LinkId.h>
#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/MakeReversibleCommandContext_Ref.h"

namespace Lab {

struct ReversibleCommand_AddLink;

struct Command_AddLink {
    Cool::LinkId link_id;
    Cool::Link   link;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto make_reversible(MakeReversibleCommandContext_Ref const& ctx) const -> ReversibleCommand_AddLink;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Link id", link_id),
            cereal::make_nvp("Link", link)
        );
    }
};

struct ReversibleCommand_AddLink {
    Command_AddLink fwd;

    void execute(CommandExecutionContext_Ref const& ctx) const;
    void revert(CommandExecutionContext_Ref const& ctx) const;
    auto to_string() const -> std::string;
    auto merge(ReversibleCommand_AddLink const&) const -> std::optional<ReversibleCommand_AddLink>;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Forward command", fwd)
        );
    }
};

} // namespace Lab