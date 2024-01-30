#pragma once
#include <Module/ModuleDependencies.h>
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/OpenGL/Shader.h"
#include "Cool/Nodes/NodesGraph.h"

namespace Lab {

void set_uniforms_for_shader_based_module(
    Cool::OpenGL::Shader const&,
    Cool::InputProvider_Ref,
    ModuleDependencies const&,
    Cool::DoubleBufferedRenderTarget const&,
    Cool::NodesGraph const&
);

} // namespace Lab