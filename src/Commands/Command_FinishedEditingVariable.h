#pragma once

#include "CommandCore/CommandExecutionContext_Ref.h"
#include "CommandCore/LAB_REGISTER_COMMAND.h"

namespace Lab {

/// Submit this when for example you finished dragging a slider (e.g. when ImGui::IsItemDeactivatedAfterEdit() is true).
/// It will make sure that the commit will be locked in the history and future commands modifying the same value
/// won't be merged with the ones that were issued before Command_FinishedEditingVariable.
struct Command_FinishedEditingVariable {
    void execute(CommandExecutionContext_Ref& ctx) const
    {
        ctx.history().dont_merge_next_command();
        // TODO(JF) Remove this hack.
        // Force recompilation of all shaders when a variable changes to allow gradient variables to update (bc they need to generate shader code)
        {
            auto&            registry = ctx.dirty_registry();
            std::unique_lock lock{registry.mutex()};
            for (auto& [_, is_dirty] : registry)
            {
                is_dirty.is_dirty = true;
            }
        }
    }

    auto to_string() const -> std::string
    {
        return "Finished editing variable";
    }
};

} // namespace Lab

namespace cereal {

template<class Archive>
void serialize(Archive&, Lab::Command_FinishedEditingVariable&)
{
}

} // namespace cereal

LAB_REGISTER_COMMAND(Lab::Command_FinishedEditingVariable)