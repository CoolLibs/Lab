#pragma once
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Dependencies/SystemValues.h>
#include <Module/ModuleDependencies.h>
#include <glm/glm.hpp>
#include <glm/gtx/component_wise.hpp>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/OpenGL/SSBO.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Dependencies/Ui.h"
#include "Nodes/NodeDefinition.h"

namespace Lab {

class Meshing_Handler {
public:

    Meshing_Handler();
    void imgui_windows(Ui_Ref const&) const;

    void set_sampling_count(unsigned int sampling_count);

    void generate_mesh_if_needed(
        Cool::DoubleBufferedRenderTarget const&     feedback_double_buffer,
        Cool::NodesGraph const&                     nodes_graph,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        SystemValues const&                         system_values,
        Cool::NodeId const&                         main_node_id
    );

private:
    inline void bind_SSBO() { _signed_distance_field.bind(); }

    void compute_mesh(
        Cool::DoubleBufferedRenderTarget const&     feedback_double_buffer,
        Cool::NodesGraph const&                     nodes_graph,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        SystemValues const&                         system_values,
        Cool::NodeId const&                         main_node_id
    );

    [[nodiscard]] inline auto get_ssbo_size() const -> unsigned int { return glm::compMul(_sampling_count); }

private:
    Cool::SSBO<float> _signed_distance_field{0};

    glm::uvec3   _sampling_count{3};
    mutable bool _needs_to_compute_mesh{false};
};

} // namespace Lab
