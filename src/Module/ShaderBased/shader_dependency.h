#pragma once

namespace Lab {

// We don't want to rerender when audio / time changes if we don't depend on them.
/// Also, audio features are costly to compute, so we only set these uniforms in the shader if we actually need them.
struct ModuleShaderDependencyFlags {
    bool depends_on_time{false};
    bool depends_on_audio_volume{false};
    bool depends_on_audio_waveform{false};
    bool depends_on_audio_spectrum{false};
    void reset();
    void compute_dependencies(std::string shader_code);
    auto depends_on_audio() const -> bool { return depends_on_audio_volume || depends_on_audio_waveform || depends_on_audio_spectrum; }
};

} // namespace Lab
