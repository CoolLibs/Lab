#pragma once
#include <Module/ModuleDependencies.h>
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/OpenGL/Shader.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Dependencies/SystemValues.h"

namespace Lab {

void set_uniforms_for_shader_based_module(
    Cool::OpenGL::Shader const&,
    SystemValues const&,
    ModuleDependencies const&,
    Cool::DoubleBufferedRenderTarget const&,
    Cool::NodesGraph&
);

} // namespace Lab