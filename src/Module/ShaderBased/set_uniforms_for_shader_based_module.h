#pragma once
#include <Module/ModuleDependencies.h>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/WebGPU/FullscreenPipelineGLSL.h"
#include "Dependencies/SystemValues.h"

namespace Lab {

void set_uniforms_for_shader_based_module(
    Cool::FullscreenPipelineGLSL const&,
    SystemValues const&,
    ModuleDependencies const&,
    Cool::DoubleBufferedRenderTarget const&,
    Cool::NodesGraph const&
);

} // namespace Lab