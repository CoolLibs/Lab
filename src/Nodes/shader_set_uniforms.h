#pragma once

#include "Cool/Gpu/DoubleBufferedRenderTarget.h"
#include "Cool/Gpu/OpenGL/Shader.h"
#include "Cool/Nodes/NodesGraph.h"
#include "Module/Module.h"

namespace Lab {

struct ModuleShaderDependencyFlags {
    bool const&                             depends_on_time;
    bool const&                             depends_on_audio_volume;
    bool const&                             depends_on_audio_waveform;
    bool const&                             depends_on_audio_spectrum;
    Cool::DoubleBufferedRenderTarget const& feedback_double_buffer;
};

auto shader_set_uniforms(Cool::OpenGL::Shader const&, Module::RenderParams const&, ModuleShaderDependencyFlags const&) -> void;
auto shader_send_uniforms(Cool::OpenGL::Shader const&, Module::RenderParams const&, Cool::NodesGraph const*) -> void;

} // namespace Lab
