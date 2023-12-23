#pragma once
#include <string>

namespace Lab {

struct ModuleShaderDependencyFlags {
    bool depends_on_time{false};
    bool depends_on_audio_volume{false};
    bool depends_on_audio_waveform{false};
    bool depends_on_audio_spectrum{false};
    auto reset() -> void;
    auto compute_dependencies(std::string const& shader_code) -> void;
};

} // namespace Lab
