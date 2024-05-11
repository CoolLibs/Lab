#include "MeshingParams.h"
#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"

namespace Meshing {

void MeshingParams::imgui()
{
    if (ImGui::InputScalarN("Sampling Count", ImGuiDataType_U32, glm::value_ptr(sampling_count), 3, nullptr, nullptr, "%u"))
    {
        // Ensure that the sampling count is at least 1
        sampling_count = glm::max(sampling_count, glm::uvec3(1));
    }
    ImGui::SetItemTooltip("The number of samples to take along each axis of the volume.");
}

} // namespace Meshing