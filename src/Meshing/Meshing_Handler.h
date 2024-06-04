#pragma once

#include <Cool/Gpu/OpenGL/SSBO.h>
#include <Cool/Log/MessageSender.h>
#include <Cool/Nodes/NodesGraph.h>
#include <Dependencies/SystemValues.h>
#include <Meshing/MeshingGui.h>
#include <Module/ModuleDependencies.h>
#include <Module_Compositing/Module_Compositing.h>
#include <glm/glm.hpp>
#include <glm/gtx/component_wise.hpp>
#include <memory>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Nodes/GetNodeDefinition_Ref.h"
#include "Nodes/NodeDefinition.h"

namespace Lab {

struct meshing_imgui_window_Params {
    SystemValues                                       project_system_values;
    Cool::DoubleBufferedRenderTarget const&            feedback_double_buffer;
    Cool::NodesGraph const&                            nodes_graph;
    Cool::GetNodeDefinition_Ref<NodeDefinition> const& get_node_definition;
};

class Meshing_Handler {
public:
    void imgui_window(meshing_imgui_window_Params const& meshing_imgui_params);

    void open_meshing_window(Cool::NodeId const& node_id);

    std::optional<Meshing::Mesh> compute_mesh(
        Cool::DoubleBufferedRenderTarget const&     feedback_double_buffer,
        Cool::NodesGraph const&                     nodes_graph,
        Cool::GetNodeDefinition_Ref<NodeDefinition> get_node_definition,
        SystemValues const&                         system_values,
        Cool::NodeId const&                         main_node_id
    ) const;

private:
private:
    std::unique_ptr<MeshingGui> _gui{std::make_unique<MeshingGui>()};
    Meshing::MeshingParams      _meshing_params{};
    Cool::NodeId                _target_node_id{};

    unsigned int _ssbo_binding{0};
};

} // namespace Lab
