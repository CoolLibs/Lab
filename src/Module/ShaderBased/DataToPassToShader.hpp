#pragma once
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Dependencies/SystemValues.h"

namespace Lab {

struct DataToPassToShader {
    SystemValues                            system_values;
    Cool::NodesGraph const&                 nodes_graph;            // NOLINT(*avoid-const-or-ref-data-members)
    Cool::DoubleBufferedRenderTarget const& feedback_double_buffer; // NOLINT(*avoid-const-or-ref-data-members)
};

} // namespace Lab