#pragma once
#include "Cool/Gpu/OpenGL/Shader.h"
#include "DataToPassToShader.hpp"
#include "Module/ModuleDependencies.h"

namespace Lab {

class ModulesGraphNode;

void set_uniforms_for_shader_based_module(
    Cool::OpenGL::Shader const&,
    ModuleDependencies const&,
    DataToPassToShader const&,
    std::vector<std::shared_ptr<ModulesGraphNode>> const& module_dependencies
);

} // namespace Lab