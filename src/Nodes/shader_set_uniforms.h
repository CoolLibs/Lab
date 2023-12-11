#pragma once

#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/OpenGL/Shader.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Module/Module.h"
#include "shader_dependency.h"

namespace Lab {

auto shader_set_uniforms(Cool::OpenGL::Shader const&, Module::RenderParams const&, ModuleShaderDependencyFlags const&, Cool::DoubleBufferedRenderTarget const&) -> void;
auto shader_send_uniforms(Cool::OpenGL::Shader const&, Module::RenderParams const&, Cool::NodesGraph const*) -> void;

} // namespace Lab