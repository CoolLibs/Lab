#pragma once
#include "Cool/Dependencies/InputProvider_Ref.h"
#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/OpenGL/Shader.h"
#include "Cool/Nodes/NodesGraph.h"
#include "shader_dependency.h"

namespace Lab {

void set_uniforms_for_shader_based_module(
    Cool::OpenGL::Shader const&,
    Cool::InputProvider_Ref,
    ModuleShaderDependencyFlags const&,
    Cool::DoubleBufferedRenderTarget const&,
    Cool::Input<Cool::Camera> const&,
    Cool::NodesGraph const&
);

} // namespace Lab