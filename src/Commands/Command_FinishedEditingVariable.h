#pragma once

#include "CommandCore/CommandExecutionContext_Ref.h"

namespace Lab {

/// Submit this when for example you finished dragging a slider (e.g. when ImGui::IsItemDeactivatedAfterEdit() is true).
/// It will make sure that the commit will be locked in the history and future commands modifying the same value
/// won't be merged with the ones that were issued before Command_FinishedEditingVariable.
struct Command_FinishedEditingVariable {
    void execute(CommandExecutionContext_Ref const& ctx) const
    {
        ctx.history().dont_merge_next_command();
    }

    auto to_string() const -> std::string
    {
        return "Finished editing variable";
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

} // namespace Lab