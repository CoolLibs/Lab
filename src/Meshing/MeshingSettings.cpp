#include "MeshingSettings.hpp"

namespace Lab {

void MeshingSettings::imgui()
{
    // TODO(Meshing) GUI for the other settings
    if (ImGui::InputScalarN("Samples Count", ImGuiDataType_U32, glm::value_ptr(samples_count), 3, nullptr, nullptr, "%u"))
    {
        // Ensure that the sampling count is at least 1
        samples_count = glm::max(samples_count, glm::uvec3(1));
    }
    ImGui::SetItemTooltip("The number of samples to take along each axis of the volume.");
}

} // namespace Lab