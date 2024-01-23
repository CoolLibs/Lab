#pragma once

namespace Lab {

// We don't want to rerender when audio / time changes if we don't depend on them.
/// Also, audio features are costly to compute, so we only set these uniforms in the shader if we actually need them.
struct ModuleDependencies {
    bool time{false};
    bool audio_volume{false};
    bool audio_waveform{false};
    bool audio_spectrum{false};

    auto audio() const -> bool { return audio_volume || audio_waveform || audio_spectrum; }
};

auto compute_dependencies(std::string shader_code) -> ModuleDependencies;

} // namespace Lab
